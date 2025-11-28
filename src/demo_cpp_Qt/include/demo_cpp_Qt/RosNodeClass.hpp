#ifndef SYS_CONTROL_HPP
#define SYS_CONTROL_HPP

#include "rclcpp/rclcpp.hpp"
#include "demo_interface/msg/system.hpp"
#include "demo_cpp_Qt/sys_node.hpp"
#include "demo_cpp_Qt/SystemSub.hpp"
// #include <rviz_common/ros_integration/ros_node_abstraction.hpp>
#include <QThread>
#include <QTreeWidget>

using namespace std;

class RosNodeClass : public QObject
{
    Q_OBJECT
private:
    rclcpp::NodeOptions options;

    rclcpp::executors::SingleThreadedExecutor *m_executor = nullptr;
    std::shared_ptr<rclcpp::Node> m_node;
    std::shared_ptr<SystemSub::MapSub::MapSub> m_mapSubNode;
    std::shared_ptr<SystemSub::NodeSub::NodeListSub> m_subNodeList;
    std::shared_ptr<SysNode> m_pubNode;
    std::thread spin_thread_;
    bool spinning_;
    void runExecutor();
    // const rclcpp::Node::SharedPtr m_rosNode = rclcpp::Node::make_shared("rviz_node");

public:
    std::vector<std::string> getTopicList(string nodename);

    void StartNode(string nodeName, string topicName);
    void StopNode(string nodeName, string topicName);
    void startNode();
    void stopNode();

    SystemSub::NodeSub::NodeListSub *getSubNodeList();
    SystemSub::MapSub::MapSub *getSubNodeMap();
    rclcpp::executors::SingleThreadedExecutor *getExecutor() { return m_executor; }
    // std::shared_ptr<rviz_common::ros_integration::RosNodeAbstraction> getRosNodeAbstraction() { return m_rosNodeAbstraction; }
    // rclcpp::Clock::SharedPtr getClock() { return m_clock; }
    static std::vector<std::string> getNodeList();

    RosNodeClass(QObject *parent = nullptr);
};

#endif // SYS_CONTROL_HPP