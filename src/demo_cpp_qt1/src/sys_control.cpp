#include "demo_cpp_qt1/sys_control.hpp"

Control::Control()
{
    options.use_intra_process_comms(true);
    auto node_sub = make_shared<SysSub>("system_sub_node", options);
    auto node_pub = make_shared<SysNode>("system_pub_node", options);
}
void Control::set_node_start()
{
    node_sub->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE);
    node_sub->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE);
}
void Control::set_node_stop()
{
    node_sub->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_DEACTIVATE);
    node_sub->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_UNCONFIGURED_SHUTDOWN);
}