#include "ui_widget.h"
#include "demo_cpp_qt2/Widget.h"
#include <QToolButton>
#include <QDateTime>

#define TABLE_COL_COUNT 6
#define INIT_TOPIC_COUNT 16

Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);

    initTable();

    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidget, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));

    QStringList valueList;
    valueList << "NodeList";
    m_rootItem = new QTreeWidgetItem(valueList);
    ui->treeWidget->addTopLevelItem(m_rootItem); // 作为顶级节点

    connect(ui->refreshNodeBtn, SIGNAL(clicked()), this, SLOT(onRefreshNodeBtnClicked()));

    connect(m_nodeclass.getSubNode(), SIGNAL(sendmsg(QString)), this, SLOT(onRecv(QString)));
}

void Widget::onRecv(QString value)
{
    ui->plainTextEdit->appendPlainText(QDateTime::currentDateTime().toString("yyMMdd hh:mm:ss") + " recv: " + value);
}

void Widget::onRefreshNodeBtnClicked()
{
    removeTreeItem(m_rootItem);
    // call get node list func
    // 获取node列表
    std::vector<std::string> nodeList = ROSNodeClass::getNodeList();
    for (int i = 0; i < nodeList.size(); i++)
    {
        QString value = QString(nodeList[i].c_str());
        QTreeWidgetItem *subNode = new QTreeWidgetItem(QStringList(value));
        m_rootItem->addChild(subNode);
    }

    ui->plainTextEdit->appendPlainText("get node list success, count is " + QString::number(nodeList.size()));

    onExpandNode();
}

void Widget::onCustomContextMenu(const QPoint &point) // 创建右键菜单的槽函数
{
    // 实例化右键菜单
    if (m_menu == nullptr)
    {
        m_menu = new QMenu;

        // 实例化右键菜单选项
        QAction *action1 = new QAction("GetTopicList");
        QAction *action2 = new QAction("Expand");
        QAction *action3 = new QAction("Collispe");

        // 连接菜单选项点击信号与槽函数
        this->connect(action1, SIGNAL(triggered(bool)), this, SLOT(onGetTopicList()));
        this->connect(action2, SIGNAL(triggered(bool)), this, SLOT(onExpandNode()));
        this->connect(action3, SIGNAL(triggered(bool)), this, SLOT(onCollispeNode()));

        m_menu->addAction(action1);
        m_menu->addAction(action2);
        m_menu->addAction(action3);
    }

    m_menu->actions()[0]->setVisible(false);
    m_menu->actions()[1]->setVisible(false);
    m_menu->actions()[2]->setVisible(false);

    // 根据pos判断鼠标右击位置是树的根节点root，还是孩子节点child，或者都不是
    // 鼠标右击的位置是树的节点，则item是对应的节点信息，否则为NULL
    m_curSelectTreeItem = ui->treeWidget->itemAt(point); // 关键
    if (m_curSelectTreeItem)
    {
        if (m_curSelectTreeItem == m_rootItem)
        {
            m_menu->actions()[1]->setVisible(true);
            m_menu->actions()[2]->setVisible(true);
        }
        else
        {
            m_menu->actions()[0]->setVisible(true);
        }
    }

    // 移动菜单出现在鼠标点击的位置
    m_menu->move(ui->treeWidget->cursor().pos());
    m_menu->show();
}

void Widget::onCollispeNode()
{
    ui->treeWidget->collapseAll();
}

void Widget::onExpandNode()
{
    ui->treeWidget->expandAll();
}
void Widget::onGetTopicList()
{
    for (int i = 0; i < INIT_TOPIC_COUNT; i++)
    {
        m_dataTableModel->item(i, 1)->setText("");
        m_dataTableModel->item(i, 2)->setText("");
        m_dataTableModel->item(i, 3)->setText("");
    }
    // get topic list
    if (m_curSelectTreeItem)
    {
        QString nodeName = m_curSelectTreeItem->text(0);
        nodeName = nodeName.remove(' ');
        nodeName = nodeName.remove('\n');
        nodeName = nodeName.remove('\r');
        nodeName = nodeName.remove('\t');
        ui->plainTextEdit->appendPlainText("cur select node is " + nodeName);
        std::vector<std::string> topicList = m_nodeclass.getTopicList(nodeName.toStdString());

        int count = topicList.size();
        if (count > INIT_TOPIC_COUNT)
            count = INIT_TOPIC_COUNT;
        for (int i = 0; i < count; i++)
        {
            QString value = QString(topicList[i].c_str());
            m_dataTableModel->item(i, 1)->setText(value);
        }
    }
}

void Widget::removeTreeItem(QTreeWidgetItem *item)
{
    int count = item->childCount();
    for (int i = 0; i < count; i++)
    {
        QTreeWidgetItem *childItem = item->child(0); // 删除子节点
        delete childItem;
    }
}

void Widget::onSubBtnClicked()
{
    QToolButton *btn = (QToolButton *)sender();
    int index = btn->toolTip().toInt();
    QString topicName = m_dataTableModel->item(index, 1)->text();

    if (btn->text().compare("Start", Qt::CaseInsensitive) == 0)
    {
        m_nodeclass.startNode();
        btn->setText("Stop");
    }
    else
    {
        m_nodeclass.stopNode();
        btn->setText("Start");
    }

    ui->plainTextEdit->appendPlainText("topic " + topicName + " clicked");
}

void Widget::initTable()
{
    m_dataTableView = new QTableView();
    m_dataTableView->setMinimumHeight(400);
    ui->tableGridLayout->addWidget(m_dataTableView);
    m_dataTableModel = new QStandardItemModel();
    m_dataTableView->setModel(m_dataTableModel);

    m_dataTableView->setAlternatingRowColors(true);
    m_dataTableView->setIconSize(QSize(38, 38));
    m_dataTableView->verticalHeader()->setHidden(true);
    m_dataTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    m_dataTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_dataTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_dataTableView->horizontalHeader()->setMinimumSectionSize(60);
    m_dataTableView->verticalHeader()->setMinimumSectionSize(46);

    m_dataTableModel->setColumnCount(TABLE_COL_COUNT);
    m_dataTableModel->setRowCount(INIT_TOPIC_COUNT);

    m_dataTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    QStringList colHeaderLabels;
    QStringList rowHeaderLabels;

    for (int i = 0; i < INIT_TOPIC_COUNT; i++)
        rowHeaderLabels << QString::number(i + 1);

    colHeaderLabels << "No"
                    << "Topic"
                    << "Value"
                    << "Time"
                    << "Operation"
                    << "  ";

    m_dataTableModel->setVerticalHeaderLabels(rowHeaderLabels);
    m_dataTableModel->setHorizontalHeaderLabels(colHeaderLabels);

    m_dataTableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);

    for (int i = 0; i < INIT_TOPIC_COUNT; i++)
    {
        for (int j = 0; j < TABLE_COL_COUNT; j++)
        {
            if (j == 4)
            {
                QToolButton *btn = new QToolButton();
                btn->setText("Start");
                btn->setToolTip(QString::number(i));
                connect(btn, SIGNAL(clicked()), this, SLOT(onSubBtnClicked()));
                m_dataTableView->setIndexWidget(m_dataTableModel->index(i, j), btn);
            }
            else
            {
                QStandardItem *item = new QStandardItem("");
                item->setTextAlignment(Qt::AlignCenter); // 设置居中对齐
                if (j == 0)
                {
                    item->setText(QString::number(i + 1));
                }

                item->setFlags(item->flags() & (~Qt::ItemIsEditable));

                m_dataTableModel->setItem(i, j, item);
            }
        }
    }

    m_dataTableView->setColumnWidth(4, 50);
    m_dataTableView->setColumnWidth(2, 200);
}

Widget::~Widget()
{
    delete ui;
}
/*
void Widget::on_label_linkActivated(const QString &link)
{
    ui->label->setText(link);
}

void Widget::setlabel(const QString &qmsg)
{

    on_label_linkActivated(qmsg);
}
void Widget::pushButton_clicked()
{
    // 外部函数传入接口
    emit PB_node_start();
}
void Widget::pushButton_2_clicked()
{
    // 外部函数传入接口
    emit PB_node_stop();
}
void Widget::treeWidget_clicked(const QModelIndex &index)
{
    if (!index.isValid())
    {
        return;
    }
    QTreeWidgetItem *item = ui->treeWidget->itemFromIndex(index);
    QString itemText = item ? item->text(index.column()) : nullptr;
    emit list_clicked(itemText.toStdString());
}
*/