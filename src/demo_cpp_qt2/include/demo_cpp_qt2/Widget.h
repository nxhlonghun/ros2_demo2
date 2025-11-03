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
#include <QFile>
#include <QMutex>
#include <QPainter>
#include <QJsonObject>
#include <QJsonArray>
#include <QToolButton>
#include <QDateTime>
// #include <rviz_common/render_panel.hpp>
// #include <rviz_common/visualization_manager.hpp>
// #include <rviz_common/display.hpp>

// #include "demo_cpp_qt2/RosSpinThread.hpp"
#include "demo_cpp_qt2/RosNodeClass.hpp"
#include "demo_cpp_qt2/CommonClass.h"
#include "demo_cpp_qt2/MapWidget.hpp"
#include "AlgoGraphicViewImageViewList.h"

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui
{
    class Widget;
}
QT_END_NAMESPACE

class AlgoGraphicViewImageViewList;

class Widget : public QWidget
{
    Q_OBJECT

public:
    // RosSpinThread *m_spinThread = nullptr;
    QString qmsg;
    Widget(QWidget *parent = nullptr);
    ~Widget();
    RosNodeClass *m_nodeclass = nullptr;
    MapWidget *map = nullptr;
private slots:

    void onLoadNodeFromFileBtnClicked();
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
    // void initRvizWidget();
    //  rviz_common::RenderPanel *m_renderPanel_;
    //  rviz_common::VisualizationManager *m_visualizationManager_;
    rclcpp::Clock::SharedPtr m_clock;
    // std::shared_ptr<rviz_common::ros_integration::RosNodeAbstraction> m_rosNodeAbstraction;

    AlgoGraphicViewImageViewList *m_imageViewList = nullptr;

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

    QJsonObject m_nodeObj;
    QJsonArray m_nodeArray;

    std::vector<std::string> list(std::vector<std::string> list);
    Ui::Widget *ui;
};
#endif // WIDGET_H
