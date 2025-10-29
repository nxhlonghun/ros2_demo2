#ifndef ROS_SPIN_THREAD_HPP
#define ROS_SPIN_THREAD_HPP
#include <QThread>
#include "rclcpp/executors/single_threaded_executor.hpp"
class RosSpinThread : public QThread
{
    Q_OBJECT
public:
    explicit RosSpinThread(rclcpp::executors::SingleThreadedExecutor *executor, QObject *parent = nullptr);

protected:
    void run() override;

private:
    rclcpp::executors::SingleThreadedExecutor *m_executor;
};
#endif // ROS_SPIN_THREAD_HPP