#pragma once

/************************************************************************
 * @auther         :
 * @date           : 2021-4
 * @description    : 布局后的显示模块
 *布局多个显示模块之间的关系
 *形成整体布局、并动态调整布局、显示信息
 ************************************************************************/

#include <QWidget>
#include <QGridLayout>
#include "AlgoGraphicView.h"

class AlgoGraphicViewImageViewList : public QWidget
{
    Q_OBJECT
public:
    explicit AlgoGraphicViewImageViewList(QWidget *parent = nullptr);
    // 设定需要的显示控件
    void SetViewCount(int num = 1);
    QList<AlgoGraphicView *> ViewList;

    void SetToFit(int CamNum = 1);
    void ClearObj(int CamNum = 1);
    void ClearPoint(int CamNum = 1);
    QPointF getCurrentViewCenter(int CamNum = 1);
    void deleteLastItem(int CamNum = 1);
    void AddItems(int CamNum = 1, AlgoGraphicViewBaseItem *item = nullptr); // 添加ROI

    QStack<AlgoGraphicViewBaseItem *> getRoiItemStack(int CamNum = 1);

    void ResetRotRect(int CamNum);
    void ResetCircle(int CamNum);
    void ResetText(int CamNum);

private:
    QGridLayout *DispLayout;
    bool Ex = false; // 全屏显示标志位
private slots:
    void Expand(QWidget *ptr);

public slots:
    // 相机标号 从1开始
    void DispInfo(int CamNum, int statue, int Result = 0, QString str = "", bool All = false);
    // 相机标号 从1开始
    void DispImage(int CamNum, QImage &img);

    void DispPoint(int CamNum, QPointF point, double len = 50, double angel = 0, QColor color = QColor(0, 255, 0));

    void DispLine(int CamNum, QVector<QPointF> &list, QColor color = QColor(0, 255, 0));

    void DispRotRect(int CamNum, double x, double y, double angel, double width, double height);

    void DispCircle(int CamNum, double x, double y, double radius);

    void DispText(int CamNum, double x, double y, QString text, int size, QColor color = QColor(255, 0, 0));
};
