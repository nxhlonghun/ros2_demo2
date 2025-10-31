#include "ui_widget.h"
#include "demo_cpp_qt2/Widget.h"
#include <QToolButton>
#include <QDateTime>
#define TABLE_COL_COUNT 6
#define INIT_TOPIC_COUNT 16

Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);

    initTable();
    // initRvizWidget();
    m_nodeclass = new RosNodeClass();
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidget, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));

    QStringList valueList;
    valueList << "NodeList";
    m_rootItem = new QTreeWidgetItem(valueList);
    ui->treeWidget->addTopLevelItem(m_rootItem); // 作为顶级节点
    QString defaultPgm = "/home/nxh/map.pgm";
    QString defaultYaml = "/home/nxh/map.yaml";
    loadMapFromFiles(defaultPgm, defaultYaml);
    connect(ui->refreshNodeBtn, SIGNAL(clicked()), this, SLOT(onRefreshNodeBtnClicked()));

    connect(m_nodeclass->getSubNodeList(), &SystemSub::NodeListSub::sendmsg, this, &Widget::onRecv, Qt::QueuedConnection);
    connect(m_nodeclass->getSubNodeMap(), &SystemSub::MapSub::poseUpDate,
            this, &Widget::onPoseUpdated, Qt::QueuedConnection);
    connect(m_nodeclass->getSubNodeMap(), &SystemSub::MapSub::laserUpDate,
            this, &Widget::onLaserUpdated, Qt::QueuedConnection);
    connect(m_nodeclass->getSubNodeMap(), &SystemSub::MapSub::occupancyGridUpDate,
            this, &Widget::onOccupancyGridUpdated, Qt::QueuedConnection);
    m_nodeclass->startNode();
}

Widget::~Widget()
{

    delete ui;
}

bool Widget::loadMapFromFiles(const QString &pgmFile, const QString &yamlFile)
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

QPoint Widget::worldToImage(double wx, double wy) const
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

QPointF Widget::robotLocalToWorld(double lx, double ly) const
{
    // robot-local (lx,ly) -> world by robot_x_, robot_y_, robot_yaw_
    double ct = qCos(robot_yaw_);
    double st = qSin(robot_yaw_);
    double wx = robot_x_ + ct * lx - st * ly;
    double wy = robot_y_ + st * lx + ct * ly;
    return QPointF(wx, wy);
}

void Widget::onPoseUpdated(double x, double y, double yaw)
{
    QMutexLocker locker(&mutex_);
    robot_x_ = x;
    robot_y_ = y;
    robot_yaw_ = yaw;
    // do not block long, request repaint
    update();
}

void Widget::onLaserUpdated(const QVector<QPointF> &points_robot)
{
    QMutexLocker locker(&mutex_);
    laser_points_robot_ = points_robot;
    // repaint
    update();
}

void Widget::onOccupancyGridUpdated(const QVector<int> &data, unsigned int width, unsigned int height,
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
    occ_image_ = QImage(occ_width_, occ_height_, QImage::Format_Grayscale8);
    for (unsigned int y = 0; y < occ_height_; ++y)
    {
        for (unsigned int x = 0; x < occ_width_; ++x)
        {
            int idx = y * occ_width_ + x;
            int v = occupancy_data_.at(idx); // -1 unknown, 0 free, 100 occ
            int gray = 255;
            if (v == -1)
                gray = 200; // unknown -> light gray
            else
            {
                // map 0..100 to 255..0
                int gv = static_cast<int>(255 - (v * 255 / 100));
                gray = qBound(0, gv, 255);
            }
            occ_image_.setPixel(x, occ_height_ - 1 - y, qRgb(gray, gray, gray)); // invert y for display match
        }
    }

    update();
}

void Widget::paintEvent(QPaintEvent * /*event*/)
{
    QLabel *mapLabel = findChild<QLabel *>("mapLabel"); // 查找子控件，确保你正确地命名了 QLabel

    // 1. 创建一个 QPixmap，用来保存绘制的图像
    QPixmap pixmap(mapLabel->size()); // 设置 pixmap 的大小和 QLabel 一致
    pixmap.fill(Qt::black);           // 填充背景色为黑色

    // 2. 使用 QPainter 绘制内容
    QPainter p(&pixmap);
    p.fillRect(pixmap.rect(), Qt::black); // 确保背景为黑色

    QMutexLocker locker(&mutex_);

    // 如果存在底图则按底图坐标绘制
    if (!baseMap_.isNull())
    {
        QImage scaledMap = baseMap_.scaled(mapLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        // 将底图居中绘制到 widget 上（也可以选择左上角(0,0)）
        int x0 = (mapLabel->width() - scaledMap.width()) / 2;
        int y0 = (mapLabel->height() - scaledMap.height()) / 2;
        p.drawImage(x0, y0, scaledMap);

        // 如果存在占据栅格图像（例如膨胀/代价值图层），则以半透明叠加绘制
        if (!occ_image_.isNull())
        {
            // 简单处理：若两者尺寸相同直接叠加，否则把 occ 缩放到底图尺寸（注意：缩放会失真，真实场景需按 origin/resolution 做精确对齐）
            if (occ_image_.size() == baseMap_.size())
            {
                p.setOpacity(0.5);
                p.drawImage(x0, y0, occ_image_);
                p.setOpacity(1.0);
            }
            else
            {
                QImage occ_scaled = occ_image_.scaled(baseMap_.size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                p.setOpacity(0.5);
                p.drawImage(x0, y0, occ_scaled);
                p.setOpacity(1.0);
            }
        }

        // 绘制激光点：先做机器人局部->世界，再 world->image，最后加上 x0,y0 偏移
        p.setPen(Qt::green);
        for (const QPointF &pt_local : laser_points_robot_)
        {
            QPointF world_pt = robotLocalToWorld(pt_local.x(), pt_local.y());
            QPoint img_pt = worldToImage(world_pt.x(), world_pt.y());
            QPoint final_pt(img_pt.x() + x0, img_pt.y() + y0);
            p.drawPoint(final_pt);
        }

        // 绘制机器人（用旋转的三角形表示朝向）
        QPoint robot_img = worldToImage(robot_x_, robot_y_);
        QPoint robot_center(robot_img.x() + x0, robot_img.y() + y0);

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
    mapLabel->setPixmap(pixmap);
    mapLabel->setScaledContents(true); // 如果需要，设置为自动缩放以适应大小 mapLabel->setPixmap(pixmap);
}

/*void Widget::initRvizWidget()
{

    m_renderPanel_ = new rviz_common::RenderPanel(ui->frame);

    QVBoxLayout *layout = new QVBoxLayout(ui->frame);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_renderPanel_);

    m_clock = make_shared<rclcpp::Clock>(RCL_ROS_TIME);
    m_rosNodeAbstraction = std::make_shared<rviz_common::ros_integration::RosNodeAbstraction>("rviz_node");

    m_visualizationManager_ = new rviz_common::VisualizationManager(m_renderPanel_, m_rosNodeAbstraction, nullptr, m_clock);

    m_renderPanel_->initialize(m_visualizationManager_);
    m_visualizationManager_->initialize();
    m_visualizationManager_->startUpdate();

    rviz_common::Display *map_display = m_visualizationManager_->createDisplay("rviz_default_plugins/Map", "Map", true);
    if (map_display)
    {
        map_display->subProp("Topic")->setValue("/map");
    }
    rviz_common::Display *laser_display = m_visualizationManager_->createDisplay("rviz_default_plugins/LaserScan", "Laser", true);
    if (map_display)
    {
        laser_display->subProp("Topic")->setValue("/scan");
    }
    rviz_common::Display *path_display = m_visualizationManager_->createDisplay("rviz_default_plugins/Path", "Path", true);
    if (map_display)
    {
        path_display->subProp("Topic")->setValue("/planned_path");
    }
}*/

void Widget::onRecv(QString value)
{
    ui->plainTextEdit->appendPlainText(QDateTime::currentDateTime().toString("yyMMdd hh:mm:ss") + " recv: " + value);
}

void Widget::onRefreshNodeBtnClicked()
{
    removeTreeItem(m_rootItem);
    // call get node list func
    // 获取node列表
    std::vector<std::string> nodeList = RosNodeClass::getNodeList();
    for (int i = 0; i < nodeList.size(); i++)
    {
        QString value = QString(nodeList[i].c_str());
        QTreeWidgetItem *subNode = new QTreeWidgetItem(QStringList(value));
        m_rootItem->addChild(subNode);
    }

    ui->plainTextEdit->appendPlainText("get node list success, count is " + QString::number(nodeList.size()));
    onExpandNode();
}

void Widget::onCustomContextMenu(const QPoint &point) // 创建右键菜单的槽函数
{
    // 实例化右键菜单
    if (m_menu == nullptr)
    {
        m_menu = new QMenu;

        // 实例化右键菜单选项
        QAction *action1 = new QAction("GetTopicList");
        QAction *action2 = new QAction("Expand");
        QAction *action3 = new QAction("Collispe");

        // 连接菜单选项点击信号与槽函数
        this->connect(action1, SIGNAL(triggered(bool)), this, SLOT(onGetTopicList()));
        this->connect(action2, SIGNAL(triggered(bool)), this, SLOT(onExpandNode()));
        this->connect(action3, SIGNAL(triggered(bool)), this, SLOT(onCollispeNode()));

        m_menu->addAction(action1);
        m_menu->addAction(action2);
        m_menu->addAction(action3);
    }

    m_menu->actions()[0]->setVisible(false);
    m_menu->actions()[1]->setVisible(false);
    m_menu->actions()[2]->setVisible(false);

    // 根据pos判断鼠标右击位置是树的根节点root，还是孩子节点child，或者都不是
    // 鼠标右击的位置是树的节点，则item是对应的节点信息，否则为NULL
    m_curSelectTreeItem = ui->treeWidget->itemAt(point); // 关键
    if (m_curSelectTreeItem)
    {
        if (m_curSelectTreeItem == m_rootItem)
        {
            m_menu->actions()[1]->setVisible(true);
            m_menu->actions()[2]->setVisible(true);
        }
        else
        {
            m_menu->actions()[0]->setVisible(true);
        }
    }

    // 移动菜单出现在鼠标点击的位置
    m_menu->move(ui->treeWidget->cursor().pos());
    m_menu->show();
}

void Widget::onCollispeNode()
{
    ui->treeWidget->collapseAll();
}

void Widget::onExpandNode()
{
    ui->treeWidget->expandAll();
}
void Widget::onGetTopicList()
{
    for (int i = 0; i < INIT_TOPIC_COUNT; i++)
    {
        m_dataTableModel->item(i, 1)->setText("");
        m_dataTableModel->item(i, 2)->setText("");
        m_dataTableModel->item(i, 3)->setText("");
    }
    // get topic list
    if (m_curSelectTreeItem)
    {
        QString nodeName = m_curSelectTreeItem->text(0);
        nodeName = nodeName.remove(' ');
        nodeName = nodeName.remove('\n');
        nodeName = nodeName.remove('\r');
        nodeName = nodeName.remove('\t');
        ui->plainTextEdit->appendPlainText("cur select node is " + nodeName);
        std::vector<std::string> topicList = m_nodeclass->getTopicList(nodeName.toStdString());

        int count = topicList.size();
        if (count > INIT_TOPIC_COUNT)
            count = INIT_TOPIC_COUNT;
        for (int i = 0; i < count; i++)
        {
            QString value = QString(topicList[i].c_str());
            m_dataTableModel->item(i, 1)->setText(value);
        }
    }
}

void Widget::removeTreeItem(QTreeWidgetItem *item)
{
    int count = item->childCount();
    for (int i = 0; i < count; i++)
    {
        QTreeWidgetItem *childItem = item->child(0); // 删除子节点
        delete childItem;
    }
}

void Widget::onSubBtnClicked()
{
    QToolButton *btn = (QToolButton *)sender();
    int index = btn->toolTip().toInt();
    QString topicName = m_dataTableModel->item(index, 1)->text();

    if (btn->text().compare("Start", Qt::CaseInsensitive) == 0)
    {
        m_nodeclass->getSubNodeList()->startNode();
        btn->setText("Stop");
    }
    else
    {
        m_nodeclass->getSubNodeList()->stopNode();
        btn->setText("Start");
    }

    ui->plainTextEdit->appendPlainText("topic " + topicName + " clicked");
}

void Widget::initTable()
{
    m_dataTableView = new QTableView();
    m_dataTableView->setMinimumHeight(400);
    ui->tableGridLayout->addWidget(m_dataTableView);
    m_dataTableModel = new QStandardItemModel();
    m_dataTableView->setModel(m_dataTableModel);

    m_dataTableView->setAlternatingRowColors(true);
    m_dataTableView->setIconSize(QSize(38, 38));
    m_dataTableView->verticalHeader()->setHidden(true);
    m_dataTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    m_dataTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_dataTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_dataTableView->horizontalHeader()->setMinimumSectionSize(60);
    m_dataTableView->verticalHeader()->setMinimumSectionSize(46);

    m_dataTableModel->setColumnCount(TABLE_COL_COUNT);
    m_dataTableModel->setRowCount(INIT_TOPIC_COUNT);

    m_dataTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    QStringList colHeaderLabels;
    QStringList rowHeaderLabels;

    for (int i = 0; i < INIT_TOPIC_COUNT; i++)
        rowHeaderLabels << QString::number(i + 1);

    colHeaderLabels << "No"
                    << "Topic"
                    << "Value"
                    << "Time"
                    << "Operation"
                    << "  ";

    m_dataTableModel->setVerticalHeaderLabels(rowHeaderLabels);
    m_dataTableModel->setHorizontalHeaderLabels(colHeaderLabels);

    m_dataTableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);

    for (int i = 0; i < INIT_TOPIC_COUNT; i++)
    {
        for (int j = 0; j < TABLE_COL_COUNT; j++)
        {
            if (j == 4)
            {
                QToolButton *btn = new QToolButton();
                btn->setText("Start");
                btn->setToolTip(QString::number(i));
                connect(btn, SIGNAL(clicked()), this, SLOT(onSubBtnClicked()));
                m_dataTableView->setIndexWidget(m_dataTableModel->index(i, j), btn);
            }
            else
            {
                QStandardItem *item = new QStandardItem("");
                item->setTextAlignment(Qt::AlignCenter); // 设置居中对齐
                if (j == 0)
                {
                    item->setText(QString::number(i + 1));
                }

                item->setFlags(item->flags() & (~Qt::ItemIsEditable));

                m_dataTableModel->setItem(i, j, item);
            }
        }
    }

    m_dataTableView->setColumnWidth(4, 50);
    m_dataTableView->setColumnWidth(2, 200);
}

/*
void Widget::on_label_linkActivated(const QString &link)
{
    ui->label->setText(link);
}

void Widget::setlabel(const QString &qmsg)
{

    on_label_linkActivated(qmsg);
}
void Widget::pushButton_clicked()
{
    // 外部函数传入接口
    emit PB_node_start();
}
void Widget::pushButton_2_clicked()
{
    // 外部函数传入接口
    emit PB_node_stop();
}
void Widget::treeWidget_clicked(const QModelIndex &index)
{
    if (!index.isValid())
    {
        return;
    }
    QTreeWidgetItem *item = ui->treeWidget->itemFromIndex(index);
    QString itemText = item ? item->text(index.column()) : nullptr;
    emit list_clicked(itemText.toStdString());
}
*/