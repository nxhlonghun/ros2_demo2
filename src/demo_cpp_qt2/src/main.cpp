
#include "demo_cpp_qt2/Widget.h"

int main(int argc, char *argv[])
{

    rclcpp::init(argc, argv);
    QApplication app(argc, argv);
    Widget w;
    // w.m_nodeclass.start();
    /*thread spin_thread([&]() -> void
                       { w.m_nodeclass.getExecutor()->spin(); });
    spin_thread.detach();*/
    w.show();
    app.exec();
    w.m_nodeclass->stopNode();
    rclcpp::shutdown();
    return 0;
}
