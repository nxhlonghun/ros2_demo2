#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QApplication>
#include <QLabel>
#include <QString>
#include <QTreeWidgetItem>
#include <QMenu>
#include <QTableView>
#include <QStandardItemModel>

#include "demo_cpp_qt2/ROSNodeClass.hpp"

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
    ROSNodeClass m_nodeclass;
    QString qmsg;
    Widget(QWidget *parent = nullptr);
    ~Widget();

    Ui::Widget *ui;

private slots:

    void onRefreshNodeBtnClicked();
    void onCustomContextMenu(const QPoint &point);
    void onCollispeNode();
    void onExpandNode();
    void onGetTopicList();
    void onSubBtnClicked();
    // void PB_clicked();

    void onRecv(QString value);

private:
    void removeTreeItem(QTreeWidgetItem *item);
    void initTable();

signals:
    void PB_node_start();
    void PB_node_stop();
    void list_clicked(string list);

private:
    QTreeWidgetItem *m_rootItem = nullptr;
    QTreeWidgetItem *m_curSelectTreeItem = nullptr;
    QMenu *m_menu = nullptr;
    QTableView *m_dataTableView = nullptr;
    QStandardItemModel *m_dataTableModel = nullptr;

    std::vector<std::string> list(std::vector<std::string> list);
};
#endif // WIDGET_H
