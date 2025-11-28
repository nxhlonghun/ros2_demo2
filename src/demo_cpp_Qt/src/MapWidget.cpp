#include "demo_cpp_Qt/MapWidget.hpp"

MapWidget::MapWidget(AlgoGraphicViewImageViewList *parent, QPushButton *button)
    : AlgoGraphicViewImageViewList(parent)
{
    // button->setCheckable(true); //
    //  mapPixmapItem->setFlags(QGraphicsItem::ItemIsMovable); // 可拖动
}

void MapWidget::onPushButtonClick(int flog)
{
    flog_ = flog;
    if (flog == 1)
    {
        QString defaultPgm = "/home/nxh/map.pgm";
        QString defaultYaml = "/home/nxh/map.yaml";
        loadMapFromFiles(defaultPgm, defaultYaml);
    }
}

void MapWidget::onSetNewNavPoint(QPointF point)
{
    flog_nav++;
    qDebug() << "导航点: " << point.x() << ", " << point.y();
    if (flog_nav == 1)
    {
        Point[0] = imageToWorld(point.x(), point.y());
    }
    else if (flog_nav == 2)
    {
        Point[1] = imageToWorld(point.x(), point.y());
        double dx = Point[1].x() - Point[0].x();
        double dy = Point[1].y() - Point[0].y();
        double yaw = std::atan2(dy, dx);
        std::cout << Point[0].x() << " " << Point[0].y() << " " << yaw << std::endl;
        emit mousePress(Point[0].x(), Point[0].y(), yaw);
        flog_nav = 0;
    }
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
            baseMap_resolution_ = line.split(":")[1].trimmed().toDouble();
        }
        else if (line.startsWith("origin:"))
        {
            QString rest = line.section('[', 1, 1).section(']', 0, 0);
            rest = rest.remove(' ');
            QStringList parts = rest.split(',');
            if (parts.size() >= 3)
            {
                baseMap_origin_x_ = parts[0].toDouble();
                baseMap_origin_y_ = parts[1].toDouble();
                baseMap_origin_theta_ = parts[2].toDouble();
            }
        }
    }
    f.close();

    // 清空 occupancy 图层
    {
        QMutexLocker locker(&mutex_);
        local_occupancy_data_.clear();
        local_occ_image_ = QImage();
        global_occupancy_data_.clear();
        global_occ_image_ = QImage();
    }

    if (flog_ == 1)
    {
        Map_ = baseMap_;
        map_resolution_ = baseMap_resolution_;
        map_origin_x_ = baseMap_origin_x_;
        map_origin_y_ = baseMap_origin_y_;
        map_origin_theta_ = baseMap_origin_theta_;
    }

    update();
    return true;
}

QPointF MapWidget::imageToWorld(double px, double py) const
{
    // 1. 像素 → 米坐标
    double ix = px * map_resolution_;
    double iy = (baseMap_.height() - py) * map_resolution_;

    // 2. 反旋转
    double ct = qCos(map_origin_theta_);
    double st = qSin(map_origin_theta_);
    double dx = ct * ix - st * iy;
    double dy = st * ix + ct * iy;

    // 3. 加回原点
    double wx = dx + map_origin_x_;
    double wy = dy + map_origin_y_;

    return QPointF(wx, wy);
}

QPointF MapWidget::worldToImage(double wx, double wy) const
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
    int py = Map_.height() - static_cast<int>(std::floor(iy / map_resolution_ + 0.5));
    return QPointF(px, py);
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

void MapWidget::onOccupancyLocalGridUpdated(const QVector<int> &data, unsigned int width, unsigned int height,
                                            double resolution, double origin_x, double origin_y, double origin_theta)
{
    QMutexLocker locker(&mutex_);
    local_occ_width_ = width;
    local_occ_height_ = height;
    local_occ_resolution_ = resolution;
    local_occ_origin_x_ = origin_x;
    local_occ_origin_y_ = origin_y;
    local_occ_origin_theta_ = origin_theta;
    local_occupancy_data_ = data;

    // build visual image (grayscale mapping)
    local_occ_image_ = QImage(local_occ_width_, local_occ_height_, QImage::Format_RGB32);
    for (unsigned int y = 0; y < local_occ_height_; ++y)
    {
        for (unsigned int x = 0; x < local_occ_width_; ++x)
        {
            int idx = y * local_occ_width_ + x;
            int v = local_occupancy_data_.at(idx); // -1 unknown, 0 free, 100 occ
            QColor color;
            if (v == -1 || v == 99)
            {
                color = QColor(51, 243, 255, 100); // unknown -> light gray
            }
            else if (v == 0)
            {
                color = QColor(255, 255, 255, 0);
            }
            else if (v == 100)
            {
                color = QColor(253, 0, 255, 255);
            }
            else if (v <= 98 && v >= 49)
            {
                int r = 255;
                int b = 255 - 255 / 49 * (v - 49);

                // int gv = static_cast<int>(255 - (v * 255 / 100));
                // gray = qBound(0, gv, 255);
                // std::cout << "v= " << v << " r = " << r << " b = " << b << std::endl;
                color = QColor(r, 0, b, 255);
            }
            else
            {
                int b = 255;
                int r = 255 - (-155 / 49 * v + 155);
                color = QColor(r, 0, b, 255);
            }
            local_occ_image_.setPixelColor(x, local_occ_height_ - 1 - y, color); // invert y for display match
        }
    }

    update();
}

void MapWidget::onOccupancyGlobalGridUpdated(const QVector<int> &data, unsigned int width, unsigned int height,
                                             double resolution, double origin_x, double origin_y, double origin_theta)
{
    QMutexLocker locker(&mutex_);
    global_occ_width_ = width;
    global_occ_height_ = height;
    global_occ_resolution_ = resolution;
    global_occ_origin_x_ = origin_x;
    global_occ_origin_y_ = origin_y;
    global_occ_origin_theta_ = origin_theta;
    global_occupancy_data_ = data;

    // build visual image (grayscale mapping)
    global_occ_image_ = QImage(global_occ_width_, global_occ_height_, QImage::Format_RGB32);
    for (unsigned int y = 0; y < global_occ_height_; ++y)
    {
        for (unsigned int x = 0; x < global_occ_width_; ++x)
        {
            int idx = y * global_occ_width_ + x;
            int v = global_occupancy_data_.at(idx); // -1 unknown, 0 free, 100 occ
            QColor color;
            if (v == -1 || v == 99)
            {
                color = QColor(51, 243, 255, 100); // unknown -> light gray
            }
            else if (v == 0)
            {
                color = QColor(255, 255, 255, 0);
            }
            else if (v == 100)
            {
                color = QColor(253, 0, 255, 255);
            }
            else if (v <= 98 && v >= 49)
            {
                int r = 255;
                int b = 255 - 255 / 49 * (v - 49);

                // int gv = static_cast<int>(255 - (v * 255 / 100));
                // gray = qBound(0, gv, 255);
                // std::cout << "v= " << v << " r = " << r << " b = " << b << std::endl;
                color = QColor(r, 0, b, 255);
            }
            else
            {
                int b = 255;
                int r = 255 - (-155 / 49 * v + 155);
                color = QColor(r, 0, b, 255);
            }
            global_occ_image_.setPixelColor(x, global_occ_height_ - 1 - y, color); // invert y for display match
        }
    }

    update();
}

void MapWidget::onLocalPathUpdated(const QVector<QPointF> &points)
{
    QMutexLocker locker(&mutex_);
    local_path_points_ = points;
    update();
}

void MapWidget::onGlobalPathUpdated(const QVector<QPointF> &points)
{
    QMutexLocker locker(&mutex_);
    global_path_points_ = points;
    update();
}

void MapWidget::onMapUpdated(const QVector<int> &data, unsigned int width, unsigned int height, double resolution, double origin_x, double origin_y, double origin_theta)
{
    QMutexLocker locker(&mutex_);
    liveMap_width_ = width;
    liveMap_height_ = height;
    liveMap_resolution_ = resolution;
    liveMap_origin_x_ = origin_x;
    liveMap_origin_y_ = origin_y;
    liveMap_origin_theta_ = origin_theta;
    liveMap_data_ = data;

    // build visual image (grayscale mapping)
    liveMap_ = QImage(liveMap_width_, liveMap_height_, QImage::Format_ARGB32);
    for (unsigned int y = 0; y < liveMap_height_; ++y)
    {
        for (unsigned int x = 0; x < liveMap_width_; ++x)
        {
            int idx = y * liveMap_width_ + x;
            int v = liveMap_data_.at(idx); // -1 unknown, 0 free, 100 occ
            QColor color;
            if (v == 100)
            {
                color = Qt::black; // unknown -> light gray
            }
            else if (v == -1)
            {
                color = QColor(255, 255, 255, 255);
            }
            else
            {
                color = Qt::gray;
            }

            liveMap_.setPixelColor(x, liveMap_height_ - 1 - y, color); // invert y for display match
        }
    }
    if (flog_ == 2)
    {
        Map_ = liveMap_;
        map_resolution_ = liveMap_resolution_;
        map_origin_x_ = liveMap_origin_x_;
        map_origin_y_ = liveMap_origin_y_;
        map_origin_theta_ = liveMap_origin_theta_;
        std::cout << "获取到动态地图 " << std::endl;
    }

    update();
}

void MapWidget::paintEvent(QPaintEvent * /*event*/)
{
    // 1. 创建一个 QPixmap，用来保存绘制的图像
    // QPixmap pixmap(mapView->viewport()->size()); // 设置 pixmap 的大小和 QLabel 一致
    // QImage pixmap(baseMap_.size(), QImage::Format_RGB32);
    // pixmap.fill(Qt::black); // 填充背景色为黑色

    // 2. 使用 QPainter 绘制内容
    // QPainter p(&pixmap);
    // p.fillRect(pixmap.rect(), Qt::black); // 确保背景为黑色
    // mapScence->clear();

    QMutexLocker locker(&mutex_);

    // 如果存在底图则按底图坐标绘制
    if (flog_ == 1)
    {
        QImage pixmap(baseMap_.size(), QImage::Format_RGB32);
        pixmap.fill(Qt::black); // 填充背景色为黑色

        // 2. 使用 QPainter 绘制内容
        QPainter p(&pixmap);
        // QImage scaledMap = baseMap_.scaled(mapView->viewport()->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QImage scaledMap = baseMap_;

        // 将底图居中绘制到 widget 上（也可以选择左上角(0,0)）
        // int x0 = (mapView->viewport()->width() - scaledMap.width()) / 2;
        // int y0 = (mapView->viewport()->height() - scaledMap.height()) / 2;
        int x0 = (scaledMap.width() - scaledMap.width()) / 2;
        int y0 = (scaledMap.height() - scaledMap.height()) / 2;
        p.drawImage(x0, y0, scaledMap);
        double scaleX = double(scaledMap.width()) / double(baseMap_.width());
        double scaleY = double(scaledMap.height()) / double(baseMap_.height());

        // 如果存在占据栅格图像（例如膨胀/代价值图层），则以半透明叠加绘制
        if (!local_occ_image_.isNull())
        {
            double sx = local_occ_resolution_ / map_resolution_ * scaleX;
            double sy = local_occ_resolution_ / map_resolution_ * scaleY;

            QImage local_occ_scaled = local_occ_image_.scaled(
                local_occ_image_.width() * sx,
                local_occ_image_.height() * sy,
                Qt::IgnoreAspectRatio,
                Qt::SmoothTransformation);

            // 将局部地图的原点转换到全局像素坐标
            QPointF robot_img = worldToImage(robot_x_, robot_y_);
            int draw_x = x0 + robot_img.x() * scaleX - local_occ_scaled.width() / 2;
            int draw_y = y0 + robot_img.y() * scaleY - local_occ_scaled.height() / 2;

            p.setOpacity(0.8);
            p.drawImage(draw_x, draw_y, local_occ_scaled);
            p.setOpacity(1.0);
        }
        if (!global_occ_image_.isNull())
        {
            double sx = global_occ_resolution_ / map_resolution_ * scaleX;
            double sy = global_occ_resolution_ / map_resolution_ * scaleY;

            QImage global_occ_scaled = global_occ_image_.scaled(
                global_occ_image_.width() * sx,
                global_occ_image_.height() * sy,
                Qt::IgnoreAspectRatio,
                Qt::SmoothTransformation);

            // 将局部地图的原点转换到全局像素坐标
            // QPoint robot_img = worldToImage(robot_x_, robot_y_);
            // int draw_x = x0 + robot_img.x() * scaleX + 1 - global_occ_scaled.width() / 2;
            // int draw_y = y0 + robot_img.y() * scaleY + 1 - global_occ_scaled.height() / 2;

            p.setOpacity(0.3);
            p.drawImage(0, 0, global_occ_scaled);
            p.setOpacity(1.0);
        }
        // 绘制激光点：先做机器人局部->世界，再 world->image，最后加上 x0,y0 偏移
        p.setPen(Qt::green);
        for (const QPointF &pt_laser : laser_points_robot_)
        {
            QPointF world_pt = robotLocalToWorld(pt_laser.x(), pt_laser.y());
            QPointF img_pt = worldToImage(world_pt.x(), world_pt.y());
            QPointF final_pt(img_pt.x() * scaleX + x0, img_pt.y() * scaleY + y0);
            p.drawPoint(final_pt);
        }

        p.setPen(Qt::green);
        for (const QPointF &pt_local : local_path_points_)
        {
            // QPointF world_pt = robotLocalToWorld(pt_local.x(), pt_local.y());
            QPointF img_pt = worldToImage(pt_local.x(), pt_local.y());
            QPointF final_pt(img_pt.x() * scaleX + x0, img_pt.y() * scaleY + y0);
            p.drawPoint(final_pt);
        }
        p.setPen(Qt::red);
        for (const QPointF &pt_global : global_path_points_)
        {
            // QPointF world_pt = robotLocalToWorld(pt_global.x(), pt_global.y());
            QPointF img_pt = worldToImage(pt_global.x(), pt_global.y());
            QPointF final_pt(img_pt.x() * scaleX + x0, img_pt.y() * scaleY + y0);
            p.drawPoint(final_pt);
        }

        // 绘制机器人（用旋转的三角形表示朝向）
        QPointF robot_img = worldToImage(robot_x_, robot_y_);
        QPointF robot_center(robot_img.x() * scaleX + x0, robot_img.y() * scaleY + y0);
        p.save();
        p.translate(robot_center);
        p.rotate(-robot_yaw_ * 180.0 / M_PI);
        static const QPoint poly[3] = {QPoint(8, 0), QPoint(-4, 4), QPoint(-4, -4)};
        p.setBrush(Qt::blue);
        p.drawConvexPolygon(poly, 3);
        p.restore();

        emit pixMapUpdate(pixmap);
    }
    else if (flog_ == 2)
    {
        QImage pixmap(liveMap_.size(), QImage::Format_RGB32);
        pixmap.fill(Qt::black); // 填充背景色为黑色

        // 2. 使用 QPainter 绘制内容
        QPainter p(&pixmap);
        // 如果没有底图，则使用 widget 中心为参考做简单绘制（仅用于调试）
        p.drawImage(0, 0, liveMap_);

        QPointF robot_img = worldToImage(robot_x_, robot_y_);
        QPointF robot_center(robot_img.x(), robot_img.y());
        p.save();
        p.translate(robot_center);
        p.rotate(-robot_yaw_ * 180.0 / M_PI);
        static const QPoint poly[3] = {QPoint(8, 0), QPoint(-4, 4), QPoint(-4, -4)};
        p.setBrush(Qt::blue);
        p.drawConvexPolygon(poly, 3);
        p.restore();

        p.setPen(Qt::green);
        for (const QPointF &pt_local : laser_points_robot_)
        {
            QPointF world_pt = robotLocalToWorld(pt_local.x(), pt_local.y());
            QPointF img_pt = worldToImage(world_pt.x(), world_pt.y());
            QPointF final_pt(img_pt.x(), img_pt.y());
            p.drawPoint(final_pt);
        }
        emit pixMapUpdate(pixmap);
    }
    else
    {
        QImage pixmap = QImage(400, 600, QImage::Format_RGB32);
        pixmap.fill(Qt::white); // 填充背景色为白色

        // 2. 使用 QPainter 绘制内容
        QPainter p(&pixmap);
        emit pixMapUpdate(pixmap);
    }
    // mapPixmapItem->setPixmap(pixmap);
    // mapPixmapItem->setPixmap(QPixmap::fromImage(pixmap));
    // emit pixMapUpdate(pixmap);
    // mapView->fitInView(mapScence->itemsBoundingRect(), Qt::KeepAspectRatio); // 如果需要，设置为自动缩放以适应大小 mapLabel->setPixmap(pixmap);
}