#include <QApplication>
#include <QLabel>
#include <QString>
#include "demo_cpp_qt0/SystemSub.hpp"
#include "rclcpp_components/register_node_macro.hpp"

namespace SystemSub
{
    namespace NodeSub
    {
        NodeListSub::NodeListSub(const rclcpp::NodeOptions &options) : QObject(nullptr), rclcpp_lifecycle::LifecycleNode("system_sub_node", options)
        {

            // label_ = new QLabel();
        }
        CallbackReturn NodeListSub::on_configure(const rclcpp_lifecycle::State &state)
        {
            if (topicName_ == "system")
            {
                subscriber_ = this->create_subscription<system_msg>(
                    "system",
                    10,
                    bind(&SystemSub::NodeSub::NodeListSub::response_callback, this, placeholders::_1));
            }
            RCLCPP_INFO(this->get_logger(), " SysSub::on_configure() is called.");
            LifecycleNode::on_configure(state);
            return CallbackReturn::SUCCESS;
        }
        CallbackReturn NodeListSub::on_activate(const rclcpp_lifecycle::State &state)
        {
            subscriber_flag_ = true;
            RCLCPP_INFO(this->get_logger(), "SysSub::on_activate() is called.");
            LifecycleNode::on_activate(state);
            return CallbackReturn::SUCCESS;
        }
        CallbackReturn NodeListSub::on_deactivate(const rclcpp_lifecycle::State &state)
        {
            if (topicName_ == "system")
            {
                subscriber_flag_ = false;
                // subscriber_.reset(); // 订阅者必须手动销毁
            }
            RCLCPP_INFO(this->get_logger(), "SysSub::on_deactivate() is called.");
            LifecycleNode::on_deactivate(state);
            return CallbackReturn::SUCCESS;
        }
        CallbackReturn NodeListSub::on_shutdown(const rclcpp_lifecycle::State &state)
        {
            subscriber_.reset(); // 订阅者必须手动销毁
            RCLCPP_INFO(this->get_logger(), "SysSub::on_shutdown() is called.");
            LifecycleNode::on_shutdown(state);
            return CallbackReturn::SUCCESS;
        }
        CallbackReturn NodeListSub::on_cleanup(const rclcpp_lifecycle::State &state)
        {
            subscriber_.reset();
            RCLCPP_INFO(this->get_logger(), "SysSub::on_shutdown() is called.");
            LifecycleNode::on_cleanup(state);
            return CallbackReturn::SUCCESS;
        }
        void NodeListSub::response_callback(const system_msg::UniquePtr msg)
        {
            if (subscriber_flag_ == false)
            {
                return;
            }
            show_str.str("");
            show_str << "节点信息:\t" << msg->node_name << "\t\n"
                     << "状态信息:\t" << msg->result << "\t\n";
            emit sendmsg(QString::fromStdString(show_str.str()));
        }
        void NodeListSub::startNode(string topicName)
        {
            // 节点初始状态进入活动状态
            topicName_ = topicName;
            RCLCPP_INFO(this->get_logger(), "节点初始状态进入活动状态");
            this->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE);
            this->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE);
        }
        void NodeListSub::stopNode(string topicName)
        {
            // 节点从活动状态返回初始状态
            topicName_ = topicName;
            RCLCPP_INFO(this->get_logger(), "节点从活动状态返回初始状态");
            this->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_DEACTIVATE);
            this->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_CLEANUP);
            topicName_.clear();
            // this->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_UNCONFIGURED_SHUTDOWN);
        }
        void NodeListSub::deactivateNode(string topicName)
        {
            // 节点从活动状态返回初始状态
            topicName_ = topicName;
            RCLCPP_INFO(this->get_logger(), "销毁节点");
            this->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_DEACTIVATE);
            this->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_CLEANUP);
            this->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_UNCONFIGURED_SHUTDOWN);
            // this->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_UNCONFIGURED_SHUTDOWN);
        }
    }
    namespace MapSub
    {
        MapSub::MapSub(const rclcpp::NodeOptions &options) : QObject(nullptr), rclcpp_lifecycle::LifecycleNode("map_sub_node", options)
        {
            qRegisterMetaType<QVector<QPointF>>("QVector<QPointF>");
            qRegisterMetaType<QVector<int>>("QVector<int>");
        }

        CallbackReturn MapSub::on_configure(const rclcpp_lifecycle::State &state)
        {
            goal_pose_pub_ = this->create_publisher<geometry_msgs::msg::PoseStamped>(
                "/goal_pose", 10);
            // amcl
            initial_pose_pub_ = this->create_publisher<geometry_msgs::msg::PoseWithCovarianceStamped>(
                "/initialpose", 10);

            // 里程计
            odom_sub_ = this->create_subscription<nav_msgs::msg::Odometry>(
                "/odom", 10,
                std::bind(&SystemSub::MapSub::MapSub::odomCallback, this, std::placeholders::_1));

            // 激光雷达
            laser_sub_ = this->create_subscription<sensor_msgs::msg::LaserScan>(
                "/scan", 10,
                std::bind(&SystemSub::MapSub::MapSub::laserCallback, this, std::placeholders::_1));

            // 占据栅格（例如局部 costmap）
            local_occupancy_sub_ = this->create_subscription<nav_msgs::msg::OccupancyGrid>(
                "/local_costmap/costmap", 1,
                std::bind(&SystemSub::MapSub::MapSub::occupancyLocalCallback, this, std::placeholders::_1));

            global_occupancy_sub_ = this->create_subscription<nav_msgs::msg::OccupancyGrid>(
                "/global_costmap/costmap", 1,
                std::bind(&SystemSub::MapSub::MapSub::occupancyGlobalCallback, this, std::placeholders::_1));

            local_path_sub_ = this->create_subscription<nav_msgs::msg::Path>(
                "/local_plan", 10,
                std::bind(&SystemSub::MapSub::MapSub::localPathCallback, this, std::placeholders::_1));
            global_path_sub_ = this->create_subscription<nav_msgs::msg::Path>(
                "/plan", 10,
                std::bind(&SystemSub::MapSub::MapSub::globalPathCallback, this, std::placeholders::_1));
            map_sub_ = this->create_subscription<nav_msgs::msg::OccupancyGrid>(
                "/map", 1,
                std::bind(&SystemSub::MapSub::MapSub::mapCallback, this, std::placeholders::_1));

            RCLCPP_INFO(this->get_logger(), "SysSub::on_configure() is called.");
            LifecycleNode::on_configure(state);
            return CallbackReturn::SUCCESS;
        }
        CallbackReturn MapSub::on_activate(const rclcpp_lifecycle::State &state)
        {
            odom_sub_flag_ = true;
            laser_sub_flag_ = true;
            local_occupancy_sub_flag_ = true;
            global_occupancy_sub_flag_ = true;
            map_sub_flag_ = true;
            local_path_sub_flag_ = true;
            global_path_sub_flag_ = true;
            goal_pose_pub_->on_activate();
            initial_pose_pub_->on_activate();
            RCLCPP_INFO(this->get_logger(), "SysSub::on_activate() is called.");
            LifecycleNode::on_activate(state);
            return CallbackReturn::SUCCESS;
        }
        CallbackReturn MapSub::on_deactivate(const rclcpp_lifecycle::State &state)
        {
            odom_sub_flag_ = false;
            laser_sub_flag_ = false;
            local_occupancy_sub_flag_ = false;
            global_occupancy_sub_flag_ = false;
            map_sub_flag_ = false;
            local_path_sub_flag_ = false;
            global_path_sub_flag_ = false;
            goal_pose_pub_->on_deactivate();
            initial_pose_pub_->on_deactivate();
            RCLCPP_INFO(this->get_logger(), "SysSub::on_deactivate() is called.");
            LifecycleNode::on_deactivate(state);
            return CallbackReturn::SUCCESS;
        }
        CallbackReturn MapSub::on_shutdown(const rclcpp_lifecycle::State &state)
        {
            odom_sub_.reset(); // 订阅者必须手动销毁
            laser_sub_.reset();
            local_occupancy_sub_.reset();
            global_occupancy_sub_.reset();
            map_sub_.reset();
            RCLCPP_INFO(this->get_logger(), "SysSub::on_shutdown() is called.");
            LifecycleNode::on_shutdown(state);
            return CallbackReturn::SUCCESS;
        }
        CallbackReturn MapSub::on_cleanup(const rclcpp_lifecycle::State &state)
        {
            odom_sub_.reset(); // 订阅者必须手动销毁
            laser_sub_.reset();
            local_occupancy_sub_.reset();
            global_occupancy_sub_.reset();
            map_sub_.reset();
            RCLCPP_INFO(this->get_logger(), "SysSub::on_shutdown() is called.");
            LifecycleNode::on_cleanup(state);
            return CallbackReturn::SUCCESS;
        }

        void MapSub::startNode()
        {
            // 节点初始状态进入活动状态
            RCLCPP_INFO(this->get_logger(), "节点初始状态进入活动状态");
            this->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE);
            this->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE);
        }
        void MapSub::stopNode()
        {
            // 节点从活动状态返回初始状态
            amcl_initialized_ = false;
            RCLCPP_INFO(this->get_logger(), "节点从活动状态返回初始状态");
            this->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_DEACTIVATE);
            this->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_CLEANUP);
            // this->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_UNCONFIGURED_SHUTDOWN);
        }
        void MapSub::deactivateNode()
        {
            // 节点从活动状态返回初始状态
            amcl_initialized_ = false;
            RCLCPP_INFO(this->get_logger(), "销毁节点");
            this->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_DEACTIVATE);
            this->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_CLEANUP);
            this->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_UNCONFIGURED_SHUTDOWN);
            // this->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_UNCONFIGURED_SHUTDOWN);
        }

        void MapSub::onmousePress(double x, double y, double yaw)
        {
            geometry_msgs::msg::PoseStamped msg;
            msg.header.stamp = this->get_clock()->now();
            msg.header.frame_id = "map";
            msg.pose.position.x = x;
            msg.pose.position.y = y;
            tf2::Quaternion q;
            q.setRPY(0, 0, yaw);
            msg.pose.orientation = tf2::toMsg(q);
            std::cout << msg.pose.position.x << " " << msg.pose.position.y << " " << yaw << std::endl;
            goal_pose_pub_->publish(msg);
        }

        void MapSub::onInitializeAMCL(int flog)
        {
            if (flog == 1)
            {
                on_initialized_ = true;
            }
            else
            {
                on_initialized_ = false;
            }
        }

        void MapSub::initializeAMCL(double x, double y, double yaw)
        {
            if (amcl_initialized_)
                return; // 只初始化一次

            geometry_msgs::msg::PoseWithCovarianceStamped msg;
            msg.header.stamp = this->get_clock()->now();
            msg.header.frame_id = "map"; // AMCL 工作在 map 坐标系

            msg.pose.pose.position.x = x;
            msg.pose.pose.position.y = y;
            msg.pose.pose.position.z = 0.0;

            tf2::Quaternion q;
            q.setRPY(0, 0, yaw);
            msg.pose.pose.orientation.x = q.x();
            msg.pose.pose.orientation.y = q.y();
            msg.pose.pose.orientation.z = q.z();
            msg.pose.pose.orientation.w = q.w();

            // 设置协方差矩阵，小值表示置信度高
            for (int i = 0; i < 36; i++)
                msg.pose.covariance[i] = 0.0;
            msg.pose.covariance[0] = 0.01;  // x
            msg.pose.covariance[7] = 0.01;  // y
            msg.pose.covariance[35] = 0.01; // yaw

            initial_pose_pub_->publish(msg);
            RCLCPP_INFO(this->get_logger(), "AMCL initialized at x=%.2f y=%.2f yaw=%.2f", x, y, yaw);

            amcl_initialized_ = true;
        }

        void MapSub::odomCallback(const nav_msgs::msg::Odometry::SharedPtr msg)
        {

            if (odom_sub_flag_ == false)
            {
                return;
            }
            // 从里程计消息中读取位置与朝向（四元数转 yaw）
            double x = msg->pose.pose.position.x + 0.1;
            double y = msg->pose.pose.position.y + 0.1;

            tf2::Quaternion q(msg->pose.pose.orientation.x,
                              msg->pose.pose.orientation.y,
                              msg->pose.pose.orientation.z,
                              msg->pose.pose.orientation.w);
            tf2::Matrix3x3 m(q);
            double roll, pitch, yaw;
            m.getRPY(roll, pitch, yaw);

            // 发出 Qt 信号（GUI 端会用 queued connection 接收）
            if (on_initialized_ == true)
            {
                initializeAMCL(x, y, yaw);
                on_initialized_ = false;
            }
            // 初始化AMCL位姿
            emit poseUpDate(x, y, yaw);
        }

        void MapSub::laserCallback(const sensor_msgs::msg::LaserScan::SharedPtr msg)
        {

            if (laser_sub_flag_ == false)
            {
                return;
            }
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

        void MapSub::mapCallback(const nav_msgs::msg::OccupancyGrid::SharedPtr msg)
        {

            if (map_sub_flag_ == false)
            {
                return;
            }
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
            emit mapUpdated(data, w, h, res, ox, oy, otheta);
        }

        void MapSub::occupancyLocalCallback(const nav_msgs::msg::OccupancyGrid::SharedPtr msg)
        {

            if (local_occupancy_sub_flag_ == false)
            {
                return;
            }
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
            emit occupancyLocalGridUpDate(data, w, h, res, ox, oy, otheta);
        }
        void MapSub::occupancyGlobalCallback(const nav_msgs::msg::OccupancyGrid::SharedPtr msg)
        {

            if (global_occupancy_sub_flag_ == false)
            {
                return;
            }
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
            emit occupancyGlobalGridUpDate(data, w, h, res, ox, oy, otheta);
        }
        void MapSub::localPathCallback(const nav_msgs::msg::Path::SharedPtr msg)
        {
            if (local_path_sub_flag_ == false)
            {
                return;
            }
            QVector<QPointF> path_points;
            for (const auto &pose_stamped : msg->poses)
            {
                double x = pose_stamped.pose.position.x;
                double y = pose_stamped.pose.position.y;
                path_points.append(QPointF(x, y));
            }
            emit localPathUpdated(path_points); // 发信号给 UI 绘制
        }
        void MapSub::globalPathCallback(const nav_msgs::msg::Path::SharedPtr msg)
        {

            if (global_path_sub_flag_ == false)
            {
                return;
            }
            QVector<QPointF> path_points;
            for (const auto &pose_stamped : msg->poses)
            {
                double x = pose_stamped.pose.position.x;
                double y = pose_stamped.pose.position.y;
                path_points.append(QPointF(x, y));
            }
            emit globalPathUpdated(path_points);
        }
    }
}

RCLCPP_COMPONENTS_REGISTER_NODE(SystemSub::NodeSub::NodeListSub)
RCLCPP_COMPONENTS_REGISTER_NODE(SystemSub::MapSub::MapSub)