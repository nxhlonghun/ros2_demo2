#ifndef SYS_CONTROL_HPP
#define SYS_CONTROL_HPP

#include "rclcpp/rclcpp.hpp"
#include "demo_interface/msg/system.hpp"
#include "demo_coo_qt2/Widget.h"
using namespace std;

class Control
{
private:
    Widget w;
    rclcpp::NodeOptions options;
    std::shared_ptr<SysSub> node_sub;
    std::shared_ptr<SysNode> node_pub;
    SysSub *node_sub_prt;
    void connect_signals();

public:
    rclcpp::executors::SingleThreadedExecutor executor;
    void set_node_start();
    void set_node_stop();
    Control(int argc, char *argv[]);
};

#endif // SYS_CONTROL_HPP