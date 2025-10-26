#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QApplication>
#include <QLabel>
#include <QString>
#include "demo_coo_qt2/sys_node_list.hpp"
#include "demo_coo_qt2/sys_node.hpp"
#include "demo_coo_qt2/sys_sub_node.hpp"

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
    Node_list node_list;
    Ui::Widget *ui;
};
#endif // WIDGET_H
