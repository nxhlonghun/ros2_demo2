/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QPushButton *refreshNodeBtn;
    QTreeWidget *treeWidget;
    QGridLayout *tableGridLayout;
    QPlainTextEdit *plainTextEdit;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName("Widget");
        Widget->resize(823, 561);
        gridLayout_2 = new QGridLayout(Widget);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        refreshNodeBtn = new QPushButton(Widget);
        refreshNodeBtn->setObjectName("refreshNodeBtn");

        gridLayout->addWidget(refreshNodeBtn, 0, 0, 1, 1);

        treeWidget = new QTreeWidget(Widget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName("treeWidget");

        gridLayout->addWidget(treeWidget, 1, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 2, 1);

        tableGridLayout = new QGridLayout();
        tableGridLayout->setObjectName("tableGridLayout");

        gridLayout_2->addLayout(tableGridLayout, 0, 1, 1, 2);

        plainTextEdit = new QPlainTextEdit(Widget);
        plainTextEdit->setObjectName("plainTextEdit");

        gridLayout_2->addWidget(plainTextEdit, 1, 1, 1, 2);

        gridLayout_2->setRowStretch(0, 1);
        gridLayout_2->setColumnStretch(1, 1);
        gridLayout_2->setColumnStretch(2, 1);

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        refreshNodeBtn->setText(QCoreApplication::translate("Widget", "RefreshNode", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
