#ifndef SYS_SUB_NODE_HPP
#define SYS_SUB_NODE_HPP

#include <QImage>

#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"

#include "demo_interface/msg/system.hpp"
#include <sensor_msgs/msg/laser_scan.hpp>
// #include <geometry_msgs/msg/transform_stamped.hpp>
#include <geometry_msgs/msg/pose_with_covariance_stamped.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <nav_msgs/msg/occupancy_grid.hpp>
#include <nav_msgs/msg/path.hpp>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>

#include <tf2/LinearMath/Quaternion.hpp>
#include <tf2/LinearMath/Matrix3x3.hpp>
#include <tf2_ros/transform_broadcaster.h>

#include <iostream>
#include <string>
#include <QObject>
#include <QVector>
#include <QPointF>

using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;
using system_msg = demo_interface::msg::System;
using namespace std;

Q_DECLARE_METATYPE(QVector<QPointF>)
Q_DECLARE_METATYPE(QVector<int>)

namespace SystemSub
{
    namespace NodeSub
    {

        class NodeListSub : public QObject, public rclcpp_lifecycle::LifecycleNode
        {
            Q_OBJECT
        private:
            rclcpp::Subscription<system_msg>::SharedPtr subscriber_;
            void response_callback(const system_msg::UniquePtr msg);

            bool subscriber_flag_ = false;
            string msg;
            stringstream show_str;
            string topicName_;

        public:
            NodeListSub(const string &node_name, const rclcpp::NodeOptions &options, QObject *parent = nullptr);
            void startNode(string topicName);
            void stopNode(string topicName);
            void deactivateNode(string topicName);

        signals:
            void sendmsg(const QString &qmsg);

        protected:
            CallbackReturn on_activate(const rclcpp_lifecycle::State &state) override;
            CallbackReturn on_deactivate(const rclcpp_lifecycle::State &state) override;
            CallbackReturn on_configure(const rclcpp_lifecycle::State &state) override;
            CallbackReturn on_shutdown(const rclcpp_lifecycle::State &state) override;
            CallbackReturn on_cleanup(const rclcpp_lifecycle::State &state) override;
        };
    }
    namespace MapSub
    {
        class MapSub : public QObject, public rclcpp_lifecycle::LifecycleNode
        {
            Q_OBJECT
        private:
            rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odom_sub_;
            rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr laser_sub_;
            rclcpp::Subscription<nav_msgs::msg::OccupancyGrid>::SharedPtr local_occupancy_sub_;
            rclcpp::Subscription<nav_msgs::msg::OccupancyGrid>::SharedPtr global_occupancy_sub_;
            rclcpp::Subscription<nav_msgs::msg::OccupancyGrid>::SharedPtr map_sub_;
            rclcpp::Subscription<nav_msgs::msg::Path>::SharedPtr local_path_sub_;
            rclcpp::Subscription<nav_msgs::msg::Path>::SharedPtr global_path_sub_;
            rclcpp_lifecycle::LifecyclePublisher<geometry_msgs::msg::PoseWithCovarianceStamped>::SharedPtr initial_pose_pub_;
            rclcpp_lifecycle::LifecyclePublisher<geometry_msgs::msg::PoseStamped>::SharedPtr goal_pose_pub_;
            bool amcl_initialized_ = false;
            bool on_initialized_ = false;
            bool odom_sub_flag_ = false;
            bool laser_sub_flag_ = false;
            bool local_occupancy_sub_flag_ = false;
            bool global_occupancy_sub_flag_ = false;
            bool map_sub_flag_ = false;
            bool local_path_sub_flag_ = false;
            bool global_path_sub_flag_ = false;

            void odomCallback(const nav_msgs::msg::Odometry::SharedPtr msg);
            void laserCallback(const sensor_msgs::msg::LaserScan::SharedPtr msg);
            void occupancyLocalCallback(const nav_msgs::msg::OccupancyGrid::SharedPtr msg);
            void occupancyGlobalCallback(const nav_msgs::msg::OccupancyGrid::SharedPtr msg);
            void mapCallback(const nav_msgs::msg::OccupancyGrid::SharedPtr msg);
            void localPathCallback(const nav_msgs::msg::Path::SharedPtr msg);
            void globalPathCallback(const nav_msgs::msg::Path::SharedPtr msg);
            void initializeAMCL(double x, double y, double yaw);

        public:
            MapSub(const string &node_name, QObject *parent = nullptr);
            void getMousePress(double x, double y, double yaw)
            {
                onmousePress(x, y, yaw);
            }
            void getInitializeAMCL(int flog)
            {
                onInitializeAMCL(flog);
            }

            void startNode();
            void stopNode();
            void deactivateNode();
            /*void startNode();
            void stopNode();*/
        signals:
            void poseUpDate(double x, double y, double yaw);
            void laserUpDate(const QVector<QPointF> &points);
            void occupancyLocalGridUpDate(const QVector<int> &data, unsigned int width, unsigned int height, double resolution, double origin_x, double origin_y, double origin_theta);
            void occupancyGlobalGridUpDate(const QVector<int> &data, unsigned int width, unsigned int height, double resolution, double origin_x, double origin_y, double origin_theta);
            void localPathUpdated(const QVector<QPointF> &points);
            void globalPathUpdated(const QVector<QPointF> &points);
            void mapUpdated(const QVector<int> &data, unsigned int width, unsigned int height, double resolution, double origin_x, double origin_y, double origin_theta);

        private slots:
            void onmousePress(double x, double y, double yaw);
            void onInitializeAMCL(int flog);

        protected:
            CallbackReturn on_activate(const rclcpp_lifecycle::State &state) override;
            CallbackReturn on_deactivate(const rclcpp_lifecycle::State &state) override;
            CallbackReturn on_configure(const rclcpp_lifecycle::State &state) override;
            CallbackReturn on_shutdown(const rclcpp_lifecycle::State &state) override;
            CallbackReturn on_cleanup(const rclcpp_lifecycle::State &state) override;
        };
    }
}
#endif