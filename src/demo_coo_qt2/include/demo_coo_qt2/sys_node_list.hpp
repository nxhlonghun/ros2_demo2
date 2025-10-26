#ifndef NODE_LIST_HPP
#define NODE_LIST_HPP

#include "rclcpp/rclcpp.hpp"
#include "demo_interface/msg/system.hpp"
#include <iostream>
#include <cstdio>
#include <array>
#include <chrono>

using namespace std;

class Node_list
{
private:
    std::vector<std::string> show_node, show_node_old;
    string result = "";
    string node_name;
    array<char, 128> buffer;

public:
    Node_list();
    string node_back();
};

#endif // NODE_LIST_HPP