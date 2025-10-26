#include "ui_widget.h"
#include "demo_cpp_qt1/Widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setLayout(ui->horizontalLayout);
    // ui->label->setText(qmsg);
    // ui->label->setText("qmsg");
    pushButton_3_clicked();
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(pushButton_clicked()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(pushButton_2_clicked()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(pushButton_3_clicked()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::pushButton_3_clicked()
{
    // 获取node列表

    QString node_info = QString::fromStdString(node_list.node_back());
    ui->label->setText(node_info);
    // on_label_linkActivated(node_info);
}

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
