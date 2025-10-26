#ifndef SYS_CONTROL_HPP
#define SYS_CONTROL_HPP

#include "rclcpp/rclcpp.hpp"
#include "demo_interface/msg/system.hpp"
#include "demo_cpp_qt1/Widget.h"
using namespace std;

class Control
{
private:
    rclcpp::executors::SingleThreadedExecutor executor;
    rclcpp::NodeOptions options;
    std::shared_ptr<SysSub> node_sub;
    std::shared_ptr<SysNode> node_pub;

public:
    void set_node_start();
    void set_node_stop();
    Control();
    void timer_callback();
};

#endif // SYS_CONTROL_HPP