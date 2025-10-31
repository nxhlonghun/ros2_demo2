#ifndef SYS_SUB_NODE_HPP
#define SYS_SUB_NODE_HPP

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "demo_interface/msg/system.hpp"
#include <sensor_msgs/msg/laser_scan.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <nav_msgs/msg/occupancy_grid.hpp>
#include <tf2/LinearMath/Quaternion.hpp>
#include <tf2/LinearMath/Matrix3x3.hpp>

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

    class NodeListSub : public QObject, public rclcpp_lifecycle::LifecycleNode
    {
        Q_OBJECT
    private:
        rclcpp::Subscription<system_msg>::SharedPtr subscriber_;
        string msg;
        void response_callback(const system_msg::UniquePtr msg);
        stringstream show_str;

    public:
        NodeListSub(const string &node_name, const rclcpp::NodeOptions &options, QObject *parent = nullptr);
        void startNode();
        void stopNode();
    signals:
        void sendmsg(const QString &qmsg);

    protected:
        CallbackReturn on_activate(const rclcpp_lifecycle::State &state) override;
        CallbackReturn on_deactivate(const rclcpp_lifecycle::State &state) override;
        CallbackReturn on_configure(const rclcpp_lifecycle::State &state) override;
        CallbackReturn on_shutdown(const rclcpp_lifecycle::State &state) override;
        CallbackReturn on_cleanup(const rclcpp_lifecycle::State &state) override;
    };

    class MapSub : public QObject, public rclcpp::Node
    {
        Q_OBJECT
    private:
        rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odom_sub_;
        rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr laser_sub_;
        rclcpp::Subscription<nav_msgs::msg::OccupancyGrid>::SharedPtr occupancy_sub_;

        void odomCallback(const nav_msgs::msg::Odometry::SharedPtr msg);
        void laserCallback(const sensor_msgs::msg::LaserScan::SharedPtr msg);
        void occupancyCallback(const nav_msgs::msg::OccupancyGrid::SharedPtr msg);

    public:
        MapSub(const string &node_name, QObject *parent = nullptr);
        /*void startNode();
        void stopNode();*/
    signals:
        void poseUpDate(double x, double y, double yaw);
        void laserUpDate(const QVector<QPointF> &points);
        void occupancyGridUpDate(const QVector<int> &data, unsigned int width, unsigned int height, double resolution, double origin_x, double origin_y, double origin_theta);

        /*protected:
            CallbackReturn on_activate(const rclcpp_lifecycle::State &state) override;
            CallbackReturn on_deactivate(const rclcpp_lifecycle::State &state) override;
            CallbackReturn on_configure(const rclcpp_lifecycle::State &state) override;
            CallbackReturn on_shutdown(const rclcpp_lifecycle::State &state) override;
            CallbackReturn on_cleanup(const rclcpp_lifecycle::State &state) override;*/
    };
}
#endif