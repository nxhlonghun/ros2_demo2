#ifndef SYS_NODE_HPP
#define SYS_NODE_HPP

#include "rclcpp/rclcpp.hpp"
#include "demo_interface/msg/system.hpp"
#include <iostream>
#include <cstdio>
#include <array>
#include <chrono>
using system_msg = demo_interface::msg::System;
using namespace std;

class SysNode : public rclcpp::Node
{
private:
    std::vector<std::string> show_node, show_node_old;
    string result = "";
    string node_name;
    array<char, 128> buffer;
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<system_msg>::SharedPtr publisher_; // 发布者的智能指针
public:
    SysNode(const string &node_name, const rclcpp::NodeOptions &options);
    void timer_callback();
};

#endif // SYS_NODE_HPP