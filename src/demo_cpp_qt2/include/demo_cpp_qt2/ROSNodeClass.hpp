#ifndef SYS_CONTROL_HPP
#define SYS_CONTROL_HPP

#include "rclcpp/rclcpp.hpp"
#include "demo_interface/msg/system.hpp"
#include "demo_cpp_qt2/sys_node.hpp"
#include "demo_cpp_qt2/sys_sub_node.hpp"
#include <QTreeWidget>

using namespace std;

class ROSNodeClass
{
private:
    rclcpp::NodeOptions options;

    rclcpp::executors::SingleThreadedExecutor *m_executor = nullptr;

    std::shared_ptr<SysSub> m_subNode;
    std::shared_ptr<SysNode> m_pubNode;

public:
    std::vector<std::string> getTopicList(string nodename);

    void startNode();
    void stopNode();

    SysSub *getSubNode();

    rclcpp::executors::SingleThreadedExecutor *getExecutor() { return m_executor; }

    static std::vector<std::string> getNodeList();

    ROSNodeClass();
};

#endif // SYS_CONTROL_HPP