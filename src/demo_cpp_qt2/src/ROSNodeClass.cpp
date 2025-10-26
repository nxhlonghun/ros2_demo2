#include "demo_cpp_qt2/ROSNodeClass.hpp"

ROSNodeClass::ROSNodeClass()
{
    m_executor = new rclcpp::executors::SingleThreadedExecutor();

    options.use_intra_process_comms(true);
    m_subNode = make_shared<SysSub>("system_sub_node", options);
    m_pubNode = make_shared<SysNode>("system_pub_node", options);

    m_executor->add_node(m_subNode->get_node_base_interface());
    m_executor->add_node(m_pubNode);
}

SysSub *ROSNodeClass::getSubNode()
{
    return m_subNode.get();
}

std::vector<std::string> ROSNodeClass::getTopicList(string nodename_)
{
    auto all_topics_and_types = m_pubNode->get_topic_names_and_types();
    std::vector<std::string> pub_topics;
    std::vector<std::string> sub_topics;
    for (const auto &topic_pair : all_topics_and_types)
    {
        const auto &topic_name = topic_pair.first;
        // cout << topic_name << endl;
        //  获取该话题的发布者信息
        auto publishers_info = m_pubNode->get_publishers_info_by_topic(topic_name);
        for (const auto &info : publishers_info)
        {

            string nodename = "/" + info.node_name();

            if (nodename == nodename_)
            {
                pub_topics.push_back(topic_name);
            }
        }

        // 获取该话题的订阅者信息
        auto subscribers_info = m_pubNode->get_subscriptions_info_by_topic(topic_name);
        for (const auto &info : subscribers_info)
        {
            string nodename = "/" + info.node_name();
            if (nodename == nodename_)
            {
                sub_topics.push_back(topic_name);
            }
        }
    }
    for (const auto &topic_list : pub_topics)
    {
        sub_topics.push_back(topic_list);
    }
    return sub_topics;
}

void ROSNodeClass::startNode()
{
    m_subNode->startNode();
}
void ROSNodeClass::stopNode()
{
    m_subNode->stopNode();
}

std::vector<std::string> ROSNodeClass::getNodeList()
{
    std::vector<std::string> nodeList;
    array<char, 128> buffer;

    FILE *pipe = popen("ros2 node list", "r");
    if (!pipe)
    {
    }
    else
    {
        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr)
        {
            nodeList.push_back(buffer.data());
        }
        pclose(pipe);
    }

    return nodeList;
}
