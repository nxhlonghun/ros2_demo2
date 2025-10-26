
#include "demo_coo_qt2/sys_control.hpp"

int main(int argc, char *argv[])
{

    rclcpp::init(argc, argv);
    QApplication app(argc, argv);
    Control control(argc, argv);
    thread spin_thread([&]() -> void
                       { control.executor.spin(); });
    spin_thread.detach();
    app.exec();
    rclcpp::shutdown();
    return 0;
}
