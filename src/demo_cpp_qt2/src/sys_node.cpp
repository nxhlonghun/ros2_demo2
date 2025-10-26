#include "demo_cpp_qt2/sys_node.hpp"

SysNode::SysNode(const string &node_name, const rclcpp::NodeOptions &options) : Node(node_name, options)
{
    publisher_ = this->create_publisher<system_msg>("system", 10);
    timer_ = this->create_wall_timer(1s, std::bind(&SysNode::timer_callback, this));
}
void SysNode::timer_callback()
{
    system_msg::UniquePtr msg(new system_msg());
    FILE *pipe = popen("ros2 node list", "r");
    if (!pipe)
    {
        msg->result = "未获取到节点信息";
    }
    else
    {
        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr)
        {
            if (find(show_node.begin(), show_node.end(), buffer.data()) == show_node.end())
            {
                show_node.push_back(buffer.data());
            }
        }
        pclose(pipe);
        // if (show_node != show_node_old)
        result.clear();
        for (const auto &node_name : show_node)
        {
            result += node_name + '\n';
        }
        show_node.clear();
        msg->node_name = result;
        msg->result = "获取到节点信息";
    }
    publisher_->publish(move(msg));
}