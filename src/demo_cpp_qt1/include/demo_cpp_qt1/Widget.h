#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "demo_cpp_qt1/sys_node_list.hpp"
#include "demo_cpp_qt1/sys_sub.hpp"

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui
{
    class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    QString qmsg;
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void setlabel(const QString &qmsg);

private slots:
    void on_label_linkActivated(const QString &link);
    void pushButton_clicked();
    void pushButton_2_clicked();
    void pushButton_3_clicked();
    // void PB_clicked();

signals:
    void PB_node_start();
    void PB_node_stop();

private:
    SysNode_list node_list;

    Ui::Widget *ui;
};
#endif // WIDGET_H
