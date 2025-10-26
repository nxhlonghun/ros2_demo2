#include <QApplication>
#include <QLabel>
#include <QString>
#include "rclcpp/rclcpp.hpp"
#include "demo_interface/msg/system.hpp"
#include "demo_cpp_qt1/Widget.h"

int main(int argc, char *argv[])
{

    rclcpp::init(argc, argv);
    QApplication app(argc, argv);
    Widget w;
    SysNode_list node_list;
    rclcpp::executors::SingleThreadedExecutor executor;
    rclcpp::NodeOptions options;
    options.use_intra_process_comms(true);
    auto node_sub = make_shared<SysSub>("system_sub_node", options);
    SysSub *node_sub_prt = node_sub.get();
    QObject::connect(&w, &Widget::PB_node_start, node_sub_prt, &SysSub::set_node_start);
    QObject::connect(&w, &Widget::PB_node_stop, node_sub_prt, &SysSub::set_node_stop);
    executor.add_node(node_sub->get_node_base_interface());
    thread spin_thread([&]() -> void
                       { executor.spin(); });
    spin_thread.detach();
    w.show();
    app.exec();
    rclcpp::shutdown();
    return 0;
}
