#ifndef SYS_SUB_NODE_HPP
#define SYS_SUB_NODE_HPP

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "demo_interface/msg/system.hpp"
#include <iostream>
#include <string>
#include <QObject>

using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;
using system_msg = demo_interface::msg::System;
using namespace std;

class SysSub : public QObject, public rclcpp_lifecycle::LifecycleNode
{
    Q_OBJECT
private:
    rclcpp::Subscription<system_msg>::SharedPtr subscriber_;
    string msg;
    void response_callback(const system_msg::UniquePtr msg);
    stringstream show_str;

public:
    SysSub(const string &node_name, const rclcpp::NodeOptions &options);
    void set_node_start();
    void set_node_stop();
signals:
    void sendmsg(const QString &qmsg);

protected:
    CallbackReturn on_activate(const rclcpp_lifecycle::State &state) override;
    CallbackReturn on_deactivate(const rclcpp_lifecycle::State &state) override;
    CallbackReturn on_configure(const rclcpp_lifecycle::State &state) override;
    CallbackReturn on_shutdown(const rclcpp_lifecycle::State &state) override;
    CallbackReturn on_cleanup(const rclcpp_lifecycle::State &state) override;
};

#endif