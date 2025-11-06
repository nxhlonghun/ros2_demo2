#include "demo_cpp_qt2/MapWidget.hpp"

MapWidget::MapWidget(QGraphicsView *displayLabel, QWidget *parent) : QWidget(parent), mapView(displayLabel)
{
    mapScence = new QGraphicsScene(this);
    mapView->setScene(mapScence);
    mapPixmapItem = mapScence->addPixmap(QPixmap());
    mapPixmapItem->setTransformationMode(Qt::SmoothTransformation); // 缩放平滑
    mapPixmapItem->setFlags(QGraphicsItem::ItemIsMovable);          // 可拖动
    QString defaultPgm = "/home/nxh/map.pgm";
    QString defaultYaml = "/home/nxh/map.yaml";
    loadMapFromFiles(defaultPgm, defaultYaml);
}

void MapWidget::wheelEvent(QWheelEvent *event)
{
    constexpr double scaleFactor = 1.15;
    if (event->angleDelta().y() > 0) // 向前滚轮放大
        mapScale_ *= scaleFactor;
    else // 向后滚轮缩小
        mapScale_ /= scaleFactor;

    // 限制缩放范围
    mapScale_ = qBound(0.1, mapScale_, 10.0);

    QTransform t;
    t.scale(mapScale_, mapScale_);
    mapView->setTransform(t);

    event->accept();
}

bool MapWidget::loadMapFromFiles(const QString &pgmFile, const QString &yamlFile)
{
    QImage img;
    if (!img.load(pgmFile))
    {
        qWarning() << "Failed to load map image:" << pgmFile;
        return false;
    }

    // 确保灰度图
    if (img.format() != QImage::Format_Grayscale8)
    {
        img = img.convertToFormat(QImage::Format_Grayscale8);
    }

    // 找到非黑像素的边界
    int left = img.width(), right = 0, top = img.height(), bottom = 0;
    for (int y = 0; y < img.height(); ++y)
    {
        const uchar *line = img.scanLine(y);
        for (int x = 0; x < img.width(); ++x)
        {
            if (line[x] > 0)
            { // 非黑像素
                if (x < left)
                    left = x;
                if (x > right)
                    right = x;
                if (y < top)
                    top = y;
                if (y > bottom)
                    bottom = y;
            }
        }
    }

    // 防止整个图都是黑色
    if (left > right || top > bottom)
    {
        qWarning() << "Map is fully black!";
        return false;
    }

    // 裁剪
    QRect cropRect(left, top, right - left + 1, bottom - top + 1);
    baseMap_ = img.copy(cropRect);

    // 解析 YAML 文件
    QFile f(yamlFile);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Failed to open yaml:" << yamlFile;
        return false;
    }
    QTextStream in(&f);
    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();
        if (line.startsWith("resolution:"))
        {
            map_resolution_ = line.split(":")[1].trimmed().toDouble();
        }
        else if (line.startsWith("origin:"))
        {
            QString rest = line.section('[', 1, 1).section(']', 0, 0);
            rest = rest.remove(' ');
            QStringList parts = rest.split(',');
            if (parts.size() >= 3)
            {
                map_origin_x_ = parts[0].toDouble();
                map_origin_y_ = parts[1].toDouble();
                map_origin_theta_ = parts[2].toDouble();
            }
        }
    }
    f.close();

    // 清空 occupancy 图层
    {
        QMutexLocker locker(&mutex_);
        occupancy_data_.clear();
        occ_image_ = QImage();
    }

    update();
    return true;
}

QPoint MapWidget::worldToImage(double wx, double wy) const
{
    // convert world (meters) to image pixel coordinates
    // step1: translate by origin
    double dx = wx - map_origin_x_;
    double dy = wy - map_origin_y_;
    // step2: rotate by -origin_theta (map origin rotates image wrt world)
    double ct = qCos(map_origin_theta_);
    double st = qSin(map_origin_theta_);
    double ix = ct * dx + st * dy;
    double iy = -st * dx + ct * dy;
    // convert to pixels (image origin top-left, y down)
    int px = static_cast<int>(std::floor(ix / map_resolution_ + 0.5));
    int py = baseMap_.height() - static_cast<int>(std::floor(iy / map_resolution_ + 0.5));
    return QPoint(px, py);
}

QPointF MapWidget::robotLocalToWorld(double lx, double ly) const
{
    // robot-local (lx,ly) -> world by robot_x_, robot_y_, robot_yaw_
    double ct = qCos(robot_yaw_);
    double st = qSin(robot_yaw_);
    double wx = robot_x_ + ct * lx - st * ly;
    double wy = robot_y_ + st * lx + ct * ly;
    return QPointF(wx, wy);
}

void MapWidget::onPoseUpdated(double x, double y, double yaw)
{
    QMutexLocker locker(&mutex_);
    robot_x_ = x;
    robot_y_ = y;
    robot_yaw_ = yaw;
    // do not block long, request repaint
    update();
}

void MapWidget::onLaserUpdated(const QVector<QPointF> &points_robot)
{
    QMutexLocker locker(&mutex_);
    laser_points_robot_ = points_robot;
    // repaint
    update();
}

void MapWidget::onOccupancyGridUpdated(const QVector<int> &data, unsigned int width, unsigned int height,
                                       double resolution, double origin_x, double origin_y, double origin_theta)
{
    QMutexLocker locker(&mutex_);
    occ_width_ = width;
    occ_height_ = height;
    occ_resolution_ = resolution;
    occ_origin_x_ = origin_x;
    occ_origin_y_ = origin_y;
    occ_origin_theta_ = origin_theta;
    occupancy_data_ = data;

    // build visual image (grayscale mapping)
    occ_image_ = QImage(occ_width_, occ_height_, QImage::Format_ARGB32);
    for (unsigned int y = 0; y < occ_height_; ++y)
    {
        for (unsigned int x = 0; x < occ_width_; ++x)
        {
            int idx = y * occ_width_ + x;
            int v = occupancy_data_.at(idx); // -1 unknown, 0 free, 100 occ
            QColor color;
            if (v == -1)
            {
                color = QColor(0, 0, 255, 100); // unknown -> light gray
            }
            else if (v == 0)
            {
                color = QColor(255, 255, 255, 0);
            }
            else if (v == 100)
            {
                color = QColor(255, 0, 0, 255);
            }
            else
            {
                // map 0..100 to 255..0
                int r = v * 255 / 100;
                int g = 255 - (v * 128 / 100);
                int b = 255 - r;
                // int gv = static_cast<int>(255 - (v * 255 / 100));
                // gray = qBound(0, gv, 255);
                color = QColor(r, g, b, 255);
            }
            occ_image_.setPixelColor(x, occ_height_ - 1 - y, color); // invert y for display match
        }
    }

    update();
}

void MapWidget::paintEvent(QPaintEvent * /*event*/)
{
    // 1. 创建一个 QPixmap，用来保存绘制的图像
    QPixmap pixmap(mapView->viewport()->size()); // 设置 pixmap 的大小和 QLabel 一致
    pixmap.fill(Qt::black);                      // 填充背景色为黑色

    // 2. 使用 QPainter 绘制内容
    QPainter p(&pixmap);
    // p.fillRect(pixmap.rect(), Qt::black); // 确保背景为黑色
    // mapScence->clear();

    QMutexLocker locker(&mutex_);

    // 如果存在底图则按底图坐标绘制
    if (!baseMap_.isNull())
    {
        QImage scaledMap = baseMap_.scaled(mapView->viewport()->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        // 将底图居中绘制到 widget 上（也可以选择左上角(0,0)）
        int x0 = (mapView->viewport()->width() - scaledMap.width()) / 2;
        int y0 = (mapView->viewport()->height() - scaledMap.height()) / 2;
        p.drawImage(x0, y0, scaledMap);
        double scaleX = double(scaledMap.width()) / double(baseMap_.width());
        double scaleY = double(scaledMap.height()) / double(baseMap_.height());

        // 如果存在占据栅格图像（例如膨胀/代价值图层），则以半透明叠加绘制
        if (!occ_image_.isNull())
        {
            double sx = occ_resolution_ / map_resolution_ * scaleX;
            double sy = occ_resolution_ / map_resolution_ * scaleY;

            QImage occ_scaled = occ_image_.scaled(
                occ_image_.width() * sx,
                occ_image_.height() * sy,
                Qt::IgnoreAspectRatio,
                Qt::SmoothTransformation);

            // 将局部地图的原点转换到全局像素坐标
            QPoint robot_img = worldToImage(robot_x_, robot_y_);
            int draw_x = x0 + robot_img.x() * scaleX - occ_scaled.width() / 2;
            int draw_y = y0 + robot_img.y() * scaleY - occ_scaled.height() / 2;

            p.setOpacity(0.5);
            p.drawImage(draw_x, draw_y, occ_scaled);
            p.setOpacity(1.0);
            // 简单处理：若两者尺寸相同直接叠加，否则把 occ 缩放到底图尺寸（注意：缩放会失真，真实场景需按 origin/resolution 做精确对齐）
            /*if (occ_image_.size() == scaledMap.size())
            {
                p.setOpacity(0.5);
                p.drawImage(x0, y0, occ_image_);
                p.setOpacity(1.0);
            }
            else
            {
                QImage occ_scaled = occ_image_.scaled(scaledMap.size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                p.setOpacity(0.5);
                p.drawImage(x0, y0, occ_scaled);
                p.setOpacity(1.0);
            }*/
        }
        // 绘制激光点：先做机器人局部->世界，再 world->image，最后加上 x0,y0 偏移
        p.setPen(Qt::green);
        for (const QPointF &pt_local : laser_points_robot_)
        {
            QPointF world_pt = robotLocalToWorld(pt_local.x(), pt_local.y());
            QPoint img_pt = worldToImage(world_pt.x(), world_pt.y());
            QPoint final_pt(img_pt.x() * scaleX + 0.5 + x0, img_pt.y() * scaleY + 0.5 + y0);
            p.drawPoint(final_pt);
        }

        // 绘制机器人（用旋转的三角形表示朝向）
        QPoint robot_img = worldToImage(robot_x_, robot_y_);
        QPoint robot_center(robot_img.x() * scaleX + 0.5 + x0, robot_img.y() * scaleY + 0.5 + y0);

        p.save();
        p.translate(robot_center);
        p.rotate(robot_yaw_ * 180.0 / M_PI);
        static const QPoint poly[3] = {QPoint(10, 0), QPoint(-6, 6), QPoint(-6, -6)};
        p.setBrush(Qt::blue);
        p.drawConvexPolygon(poly, 3);
        p.restore();
    }
    else
    {
        // 如果没有底图，则使用 widget 中心为参考做简单绘制（仅用于调试）
        p.setPen(Qt::green);
        QPoint center(width() / 2, height() / 2);
        /*for (const QPointF &pt_local : laser_points_robot_)
        {
            QPointF world_pt = robotLocalToWorld(pt_local.x(), pt_local.y());
            // 演示用：1m -> 50px 近似映射
            QPoint final(center.x() + int(world_pt.x() * 50), center.y() - int(world_pt.y() * 50));
            p.drawPoint(final);
        }*/
    }
    mapPixmapItem->setPixmap(pixmap);
    // mapView->fitInView(mapScence->itemsBoundingRect(), Qt::KeepAspectRatio); // 如果需要，设置为自动缩放以适应大小 mapLabel->setPixmap(pixmap);
}