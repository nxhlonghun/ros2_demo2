/********************************************************************************
** Form generated from reading UI file 'widget（复件）.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_FF08__590D__4EF6__FF09__H
#define UI_WIDGET_FF08__590D__4EF6__FF09__H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLineEdit *lineEdit;
    QTreeWidget *treeWidget;
    QLabel *label;
    QLineEdit *lineEdit_2;
    QVBoxLayout *verticalLayout_2;
    QPushButton *pushButton;
    QPushButton *pushButton_3;
    QPushButton *pushButton_2;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName("Widget");
        Widget->resize(800, 600);
        widget = new QWidget(Widget);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(20, 10, 381, 231));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName("verticalLayout");
        lineEdit = new QLineEdit(widget);
        lineEdit->setObjectName("lineEdit");

        verticalLayout->addWidget(lineEdit);

        treeWidget = new QTreeWidget(widget);
        treeWidget->setObjectName("treeWidget");

        verticalLayout->addWidget(treeWidget);

        label = new QLabel(widget);
        label->setObjectName("label");

        verticalLayout->addWidget(label);

        lineEdit_2 = new QLineEdit(widget);
        lineEdit_2->setObjectName("lineEdit_2");

        verticalLayout->addWidget(lineEdit_2);

        verticalLayout->setStretch(0, 1);
        verticalLayout->setStretch(1, 5);
        verticalLayout->setStretch(2, 5);
        verticalLayout->setStretch(3, 1);

        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        pushButton = new QPushButton(widget);
        pushButton->setObjectName("pushButton");

        verticalLayout_2->addWidget(pushButton);

        pushButton_3 = new QPushButton(widget);
        pushButton_3->setObjectName("pushButton_3");

        verticalLayout_2->addWidget(pushButton_3);

        pushButton_2 = new QPushButton(widget);
        pushButton_2->setObjectName("pushButton_2");

        verticalLayout_2->addWidget(pushButton_2);


        horizontalLayout->addLayout(verticalLayout_2);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        lineEdit->setText(QCoreApplication::translate("Widget", "===========\344\277\241\346\201\257\346\230\276\347\244\272===========", nullptr));
        label->setText(QCoreApplication::translate("Widget", "TextLabel", nullptr));
        lineEdit_2->setText(QCoreApplication::translate("Widget", "===========\344\277\241\346\201\257\346\230\276\347\244\272===========", nullptr));
        pushButton->setText(QCoreApplication::translate("Widget", "\350\256\242\351\230\205\350\200\205:\345\274\200", nullptr));
        pushButton_3->setText(QCoreApplication::translate("Widget", "\346\233\264\346\226\260", nullptr));
        pushButton_2->setText(QCoreApplication::translate("Widget", "\350\256\242\351\230\205\350\200\205:\345\205\263", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_FF08__590D__4EF6__FF09__H
