#include "demo_coo_qt2/sys_control.hpp"

Control::Control(int argc, char *argv[])
{
    // rclcpp::init(argc, argv);
    options.use_intra_process_comms(true);
    node_sub = make_shared<SysSub>("system_sub_node", options);
    node_pub = make_shared<SysNode>("system_pub_node", options);
    node_sub_prt = node_sub.get();
    connect_signals();
    executor.add_node(node_sub->get_node_base_interface());
    executor.add_node(node_pub);
    w.show();
}
void Control::connect_signals()
{
    QObject::connect(node_sub_prt, &SysSub::sendmsg, &w, &Widget::setlabel);
    QObject::connect(&w, &Widget::PB_node_start, node_sub_prt, &SysSub::set_node_start);
    QObject::connect(&w, &Widget::PB_node_stop, node_sub_prt, &SysSub::set_node_stop);
}