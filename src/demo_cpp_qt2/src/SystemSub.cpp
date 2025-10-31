#include <QApplication>
#include <QLabel>
#include <QString>
#include "demo_cpp_qt2/SystemSub.hpp"

SystemSub::NodeListSub::NodeListSub(const string &node_name, const rclcpp::NodeOptions &options, QObject *parent) : QObject(parent), rclcpp_lifecycle::LifecycleNode(node_name, options)
{

    // label_ = new QLabel();
}
CallbackReturn SystemSub::NodeListSub::on_configure(const rclcpp_lifecycle::State &state)
{
    subscriber_ = this->create_subscription<system_msg>("system", 10, bind(&SystemSub::NodeListSub::response_callback, this, placeholders::_1));
    RCLCPP_INFO(this->get_logger(), "SysSub::on_configure() is called.");
    return CallbackReturn::SUCCESS;
}
CallbackReturn SystemSub::NodeListSub::on_activate(const rclcpp_lifecycle::State &state)
{
    RCLCPP_INFO(this->get_logger(), "SysSub::on_activate() is called.");
    return CallbackReturn::SUCCESS;
}
CallbackReturn SystemSub::NodeListSub::on_deactivate(const rclcpp_lifecycle::State &state)
{
    subscriber_.reset(); // 订阅者必须手动销毁
    RCLCPP_INFO(this->get_logger(), "SysSub::on_deactivate() is called.");
    return CallbackReturn::SUCCESS;
}
CallbackReturn SystemSub::NodeListSub::on_shutdown(const rclcpp_lifecycle::State &state)
{
    RCLCPP_INFO(this->get_logger(), "SysSub::on_shutdown() is called.");
    return CallbackReturn::SUCCESS;
}
CallbackReturn SystemSub::NodeListSub::on_cleanup(const rclcpp_lifecycle::State &state)
{
    RCLCPP_INFO(this->get_logger(), "SysSub::on_shutdown() is called.");
    return CallbackReturn::SUCCESS;
}
void SystemSub::NodeListSub::response_callback(const system_msg::UniquePtr msg)
{
    show_str.str("");
    show_str << "节点信息:\t" << msg->node_name << "\t\n"
             << "状态信息:\t" << msg->result << "\t\n";
    emit sendmsg(QString::fromStdString(show_str.str()));
}
void SystemSub::NodeListSub::startNode()
{
    // 节点初始状态进入活动状态
    RCLCPP_INFO(this->get_logger(), "节点初始状态进入活动状态");
    this->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE);

    rclcpp_lifecycle::State state = this->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE);
}
void SystemSub::NodeListSub::stopNode()
{
    // 节点从活动状态返回初始状态
    RCLCPP_INFO(this->get_logger(), "节点从活动状态返回初始状态");

    this->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_DEACTIVATE);
    this->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_CLEANUP);
    // this->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_UNCONFIGURED_SHUTDOWN);
}

SystemSub::MapSub::MapSub(const string &node_name, QObject *parent) : QObject(parent), rclcpp::Node(node_name)
{
    qRegisterMetaType<QVector<QPointF>>("QVector<QPointF>");
    qRegisterMetaType<QVector<int>>("QVector<int>");

    odom_sub_ = this->create_subscription<nav_msgs::msg::Odometry>(
        "/odom", 10,
        std::bind(&SystemSub::MapSub::odomCallback, this, std::placeholders::_1));

    // 激光雷达
    laser_sub_ = this->create_subscription<sensor_msgs::msg::LaserScan>(
        "/scan", 10,
        std::bind(&SystemSub::MapSub::laserCallback, this, std::placeholders::_1));

    // 占据栅格（例如局部 costmap）
    occupancy_sub_ = this->create_subscription<nav_msgs::msg::OccupancyGrid>(
        "/local_costmap/costmap", 1,
        std::bind(&SystemSub::MapSub::occupancyCallback, this, std::placeholders::_1));
}
void SystemSub::MapSub::odomCallback(const nav_msgs::msg::Odometry::SharedPtr msg)
{
    // 从里程计消息中读取位置与朝向（四元数转 yaw）
    double x = msg->pose.pose.position.x;
    double y = msg->pose.pose.position.y;

    tf2::Quaternion q(msg->pose.pose.orientation.x,
                      msg->pose.pose.orientation.y,
                      msg->pose.pose.orientation.z,
                      msg->pose.pose.orientation.w);
    tf2::Matrix3x3 m(q);
    double roll, pitch, yaw;
    m.getRPY(roll, pitch, yaw);

    // 发出 Qt 信号（GUI 端会用 queued connection 接收）
    emit poseUpDate(x, y, yaw);
}

void SystemSub::MapSub::laserCallback(const sensor_msgs::msg::LaserScan::SharedPtr msg)
{
    // 将激光扫描的每个点转换为机器人局部笛卡尔坐标 (lx, ly)
    QVector<QPointF> pts;
    pts.reserve(msg->ranges.size());

    double angle = msg->angle_min;
    for (size_t i = 0; i < msg->ranges.size(); ++i)
    {
        float r = msg->ranges[i];
        if (std::isfinite(r) && r >= msg->range_min && r <= msg->range_max)
        {
            double lx = r * std::cos(angle);
            double ly = r * std::sin(angle);
            // 这里我们把点保留在机器人本地坐标系；UI 使用当前机器人位姿做 local->world 变换再绘制
            pts.append(QPointF(lx, ly));
        }
        angle += msg->angle_increment;
    }
    emit laserUpDate(pts);
}

void SystemSub::MapSub::occupancyCallback(const nav_msgs::msg::OccupancyGrid::SharedPtr msg)
{
    // 读取占据栅格的元信息与数据，然后发出信号给 UI
    unsigned int w = msg->info.width;
    unsigned int h = msg->info.height;
    double res = msg->info.resolution;
    double ox = msg->info.origin.position.x;
    double oy = msg->info.origin.position.y;
    double otheta = 0.0;
    // origin 可能有旋转，这里把 orientation 转为 yaw
    {
        tf2::Quaternion q(msg->info.origin.orientation.x,
                          msg->info.origin.orientation.y,
                          msg->info.origin.orientation.z,
                          msg->info.origin.orientation.w);
        tf2::Matrix3x3 m(q);
        double roll, pitch, yaw;
        m.getRPY(roll, pitch, yaw);
        otheta = yaw;
    }

    QVector<int> data;
    data.resize(w * h);
    for (size_t i = 0; i < msg->data.size(); ++i)
    {
        data[i] = static_cast<int>(msg->data[i]); // -1 表示未知, 0 表示空地, 100 表示占据
    }
    emit occupancyGridUpDate(data, w, h, res, ox, oy, otheta);
}