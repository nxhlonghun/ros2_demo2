#include "ui_widget.h"
#include "demo_cpp_Qt/Widget.h"
#define TABLE_COL_COUNT 6
#define INIT_TOPIC_COUNT 16

Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);

    initTable();
    // initRvizWidget();
    m_nodeclass = new RosNodeClass();
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidget, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));

    ui->pushButton_move->setCheckable(true);
    ui->pushButton_create->setCheckable(true);

    QStringList valueList;
    valueList << "NodeList" << "state";
    m_rootItem = new QTreeWidgetItem(valueList);
    ui->treeWidget->addTopLevelItem(m_rootItem); // 作为顶级节点

    connect(ui->refreshNodeBtn, SIGNAL(clicked()), this, SLOT(onRefreshNodeBtnClicked()));
    connect(ui->loadNodeFromFileBtn, SIGNAL(clicked()), this, SLOT(onLoadNodeFromFileBtnClicked()));

    connect(m_nodeclass->getSubNodeList(), &SystemSub::NodeListSub::sendmsg, this, &Widget::onRecv, Qt::QueuedConnection);
    // map = new MapWidget(ui->mapView, this);

    // qDebug() << "step1";

    map = new MapWidget();
    map->SetViewCount(1);
    ui->verticalLayout->addWidget(map);
    connect(m_nodeclass->getSubNodeMap(), &SystemSub::MapSub::poseUpDate,
            map, &MapWidget::getPoseUpdated, Qt::QueuedConnection);

    connect(m_nodeclass->getSubNodeMap(), &SystemSub::MapSub::laserUpDate,
            map, &MapWidget::getLaserUpdated, Qt::QueuedConnection);

    connect(m_nodeclass->getSubNodeMap(), &SystemSub::MapSub::occupancyLocalGridUpDate,
            map, &MapWidget::getOccupancyLocalGridUpdated, Qt::QueuedConnection);

    connect(m_nodeclass->getSubNodeMap(), &SystemSub::MapSub::occupancyGlobalGridUpDate,
            map, &MapWidget::getOccupancyGlobalGridUpdated, Qt::QueuedConnection);

    connect(m_nodeclass->getSubNodeMap(), &SystemSub::MapSub::localPathUpdated,
            map, &MapWidget::getLocalPathUpdated, Qt::QueuedConnection);

    connect(m_nodeclass->getSubNodeMap(), &SystemSub::MapSub::globalPathUpdated,
            map, &MapWidget::getGlobalPathUpdated, Qt::QueuedConnection);

    connect(m_nodeclass->getSubNodeMap(), &SystemSub::MapSub::mapUpdated,
            map, &MapWidget::getMapUpdated, Qt::QueuedConnection);
    connect(map, &MapWidget::mousePress,
            m_nodeclass->getSubNodeMap(), &SystemSub::MapSub::getMousePress, Qt::QueuedConnection);
    connect(map, &MapWidget::pixMapUpdate, this, [this](QImage pixmap_)
            { map->DispImage(1, pixmap_); }, Qt::QueuedConnection);

    connect(ui->pushButton_create, &QPushButton::clicked,
            this, &Widget::onPushButton, Qt::QueuedConnection);
    connect(ui->pushButton_move, &QPushButton::clicked,
            this, &Widget::onPushButton, Qt::QueuedConnection);
    connect(this, &Widget::pushButtonFlog,
            map, &MapWidget::getPushButtonClick, Qt::QueuedConnection);
    connect(this, &Widget::pushButtonFlog,
            m_nodeclass->getSubNodeMap(), &SystemSub::MapSub::getInitializeAMCL, Qt::QueuedConnection);
    // m_imageViewList->DispImage(1, map->pixmap_);
    m_nodeclass->startNode();

    // qDebug() << "step2";
}

void Widget::onPushButton()
{
    int flog = 0;
    if (ui->pushButton_move->isChecked() == true)
    {
        flog = 1;
        ui->pushButton_create->setChecked(false);
    }
    else if (ui->pushButton_create->isChecked() == true)
    {
        flog = 2;
        ui->pushButton_move->setChecked(false);
    }
    emit pushButtonFlog(flog);
}

void Widget::onLoadNodeFromFileBtnClicked()
{
    m_nodeObj = CommonClass::loadJsonFileObject("/home/nxh/文档/nodeConfig.json");

    m_nodeArray = m_nodeObj["nodeList"].toArray();

    removeTreeItem(m_rootItem);
    // call get node list func
    // 获取node列表
    for (int i = 0; i < m_nodeArray.size(); i++)
    {
        QJsonObject nodeObj = m_nodeArray[i].toObject();
        QStringList valueList;
        valueList << nodeObj["name"].toString();
        QTreeWidgetItem *subNode = new QTreeWidgetItem(valueList);
        m_rootItem->addChild(subNode);
        /* m_nodeTopic = nodeObj["topicList"].toArray();
        for (int i = 0; i < m_nodeTopic.size(); i++)
        {
            QString topicList;
            topicList = m_nodeTopic[i].toString();
            // QTreeWidgetItem *subNode = new QTreeWidgetItem(topicList);
            m_dataTableModel->item(i, 1)->setText(topicList);
        } */
    }

    onRefreshNodeBtnClicked();

    ui->plainTextEdit->appendPlainText("get node list success, count is " + QString::number(m_nodeArray.size()));
    onExpandNode();
}

void Widget::onRefreshNodeBtnClicked()
{
    // call get node list func
    // 获取node列表
    std::vector<std::string> nodeList = RosNodeClass::getNodeList();

    for (int k = 0; k < m_nodeArray.size(); k++)
    {
        QJsonObject nodeObj = m_nodeArray[k].toObject();
        QString nodeName = nodeObj["name"].toString();

        QTreeWidgetItem *subNode = m_rootItem->child(k);

        if (subNode)
        {
            for (int i = 0; i < nodeList.size(); i++)
            {
                QString onlneNodeName = QString(nodeList[i].c_str());
                onlneNodeName = onlneNodeName.remove(' ');
                onlneNodeName = onlneNodeName.remove('\n');
                onlneNodeName = onlneNodeName.remove('\r');
                onlneNodeName = onlneNodeName.remove('\t');

                if (nodeName == onlneNodeName)
                {
                    subNode->setText(0, nodeName + " online");
                    break;
                }
                else
                {
                    subNode->setText(0, nodeName + " offline");
                }
            }
        }
    }

    ui->plainTextEdit->appendPlainText("get node list success, count is " + QString::number(nodeList.size()));
    for (int i = 0; i < nodeList.size(); i++)
    {
        ui->plainTextEdit->appendPlainText(QString(nodeList[i].c_str()));
    }
}

Widget::~Widget()
{

    delete ui;
}

void Widget::onRecv(QString value)
{
    ui->plainTextEdit->appendPlainText(QDateTime::currentDateTime().toString("yyMMdd hh:mm:ss") + " recv: " + value);
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
        QString nodeName_list = m_curSelectTreeItem->text(0);
        nodeName_list = nodeName_list.remove(' ');
        nodeName_list = nodeName_list.remove('\n');
        nodeName_list = nodeName_list.remove('\r');
        nodeName_list = nodeName_list.remove('\t');
        nodeName_list = nodeName_list.remove("online");
        ui->plainTextEdit->appendPlainText("cur select node is " + nodeName_list);
        std::vector<std::string> topicList = m_nodeclass->getTopicList(nodeName_list.toStdString());

        int count = topicList.size();
        if (count > INIT_TOPIC_COUNT)
            count = INIT_TOPIC_COUNT;
        for (int k = 0; k < m_nodeArray.size(); k++)
        {
            QJsonObject nodeObj = m_nodeArray[k].toObject();
            QString nodeName = nodeObj["name"].toString();
            if (nodeName_list == nodeName)
            {
                m_topicArray = nodeObj["topicList"].toArray();
                for (int i = 0; i < m_topicArray.size(); i++)
                {
                    QString topicName = m_topicArray[i].toString();
                    // QString topicName = topicObj["topicList"].toString();
                    m_dataTableModel->item(i, 1)->setText(topicName);
                }
            }
        }

        /* for (int i = 0; i < count; i++)
        {
            QString value = QString(topicList[i].c_str());
            m_dataTableModel->item(i, 1)->setText(value);
        } */
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
    QString nodeName = m_curSelectTreeItem->text(0);
    QString topicName = m_dataTableModel->item(index, 1)->text();
    nodeName = nodeName.remove(' ');
    nodeName = nodeName.remove('\n');
    nodeName = nodeName.remove('\r');
    nodeName = nodeName.remove('\t');
    nodeName = nodeName.remove("online");

    if (btn->text().compare("Start", Qt::CaseInsensitive) == 0)
    {

        m_nodeclass->StartNode(nodeName.toStdString(), topicName.toStdString());
        btn->setText("Stop");
    }
    else
    {
        m_nodeclass->StopNode(nodeName.toStdString(), topicName.toStdString());
        btn->setText("Start");
    }
    std::cout << "node:" << nodeName.toStdString() << " topic:" << topicName.toStdString() << endl;
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