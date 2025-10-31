#include "demo_cpp_qt2/RosSpinThread.hpp"

RosSpinThread::RosSpinThread(rclcpp::executors::SingleThreadedExecutor *executor, QObject *parent)
    : QThread(parent), m_executor(executor)
{
}
void RosSpinThread::run()
{
    if (m_executor)
    {
        m_executor->spin();
    }
}
