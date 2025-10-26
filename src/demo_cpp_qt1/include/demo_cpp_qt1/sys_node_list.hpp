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

class SysNode_list
{
private:
    string result = "";
    string node_name;
    array<char, 128> buffer;

public:
    SysNode_list();
    string node_back();
};

#endif // SYS_NODE_HPP