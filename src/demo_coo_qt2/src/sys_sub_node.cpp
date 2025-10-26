#include <QApplication>
#include <QLabel>
#include <QString>
#include "demo_coo_qt2/sys_sub_node.hpp"

SysSub::SysSub(const string &node_name, const rclcpp::NodeOptions &options) : rclcpp_lifecycle::LifecycleNode(node_name, options)
{
    // label_ = new QLabel();
}
CallbackReturn SysSub::on_configure(const rclcpp_lifecycle::State &state)
{
    subscriber_ = this->create_subscription<system_msg>("system", 10, bind(&SysSub::response_callback, this, placeholders::_1));
    RCLCPP_INFO(this->get_logger(), "SysSub::on_configure() is called.");
    return CallbackReturn::SUCCESS;
}
CallbackReturn SysSub::on_activate(const rclcpp_lifecycle::State &state)
{
    RCLCPP_INFO(this->get_logger(), "SysSub::on_activate() is called.");
    return CallbackReturn::SUCCESS;
}
CallbackReturn SysSub::on_deactivate(const rclcpp_lifecycle::State &state)
{
    subscriber_.reset(); // 订阅者必须手动销毁
    RCLCPP_INFO(this->get_logger(), "SysSub::on_deactivate() is called.");
    return CallbackReturn::SUCCESS;
}
CallbackReturn SysSub::on_shutdown(const rclcpp_lifecycle::State &state)
{
    RCLCPP_INFO(this->get_logger(), "SysSub::on_shutdown() is called.");
    return CallbackReturn::SUCCESS;
}
CallbackReturn SysSub::on_cleanup(const rclcpp_lifecycle::State &state)
{
    RCLCPP_INFO(this->get_logger(), "SysSub::on_shutdown() is called.");
    return CallbackReturn::SUCCESS;
}
void SysSub::response_callback(const system_msg::UniquePtr msg)
{
    show_str.str("");
    show_str << "节点信息:\t" << msg->node_name << "\t\n"
             << "状态信息:\t" << msg->result << "\t\n";
    emit sendmsg(QString::fromStdString(show_str.str()));
}
void SysSub::set_node_start()
{
    // 节点初始状态进入活动状态
    this->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE);
    this->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE);
}
void SysSub::set_node_stop()
{
    // 节点从活动状态返回初始状态
    this->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_DEACTIVATE);
    this->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_CLEANUP);
    // this->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_UNCONFIGURED_SHUTDOWN);
}