
#include "demo_coo_qt2/sys_node_list.hpp"

Node_list::Node_list()
{
}
string Node_list::node_back()
{
    FILE *pipe = popen("ros2 node list", "r");
    if (!pipe)
    {
        result = "未获取到节点信息";
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
        node_name = result;
        result = "获取到节点信息";
    }
    stringstream show_str;
    show_str << "节点信息:\t" << node_name << "\t\n"
             << "状态信息:\t" << result << "\t\n";
    return show_str.str();
}