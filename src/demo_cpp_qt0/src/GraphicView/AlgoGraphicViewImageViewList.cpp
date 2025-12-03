#include "AlgoGraphicViewImageViewList.h"

AlgoGraphicViewImageViewList::AlgoGraphicViewImageViewList(QWidget *parent) : QWidget(parent)
{
    QPalette pal(this->palette());
    // pal.setColor(QPalette::Background, QColor(50,50,50)); //设置背景黑色
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    DispLayout = new QGridLayout();
    DispLayout->setSpacing(2); // 间隔
    // DispLayout->setMargin(2);//间隙
    this->setLayout(DispLayout);

    SetViewCount(1);
}
// 动态设定显示区域
void AlgoGraphicViewImageViewList::SetViewCount(int num)
{
    if (num < 0)
        return;
    if (num > ViewList.size()) // 新增显示窗口
    {
        // 清空布局
        for (int i = 0; i < ViewList.size(); i++)
        {
            DispLayout->removeWidget(ViewList[i]);
        }
        // 新增一个
        for (int i = ViewList.size(); i < num; i++)
        {
            ViewList.push_back(new AlgoGraphicView);
            connect(ViewList[ViewList.size() - 1], &AlgoGraphicView::Expand, this, &AlgoGraphicViewImageViewList::Expand);

            connect(ViewList[ViewList.size() - 1], &AlgoGraphicView::newNavPointSignal, this, &AlgoGraphicViewImageViewList::onSetNewNavPoint);
        }
        // 重新布局
        int count = ViewList.size();
        int ct = count <= 4 ? 2 : (count <= 6 ? 3 : 4); // 动态调整布局
        for (int i = 0; i < ViewList.size(); i++)
        {
            DispLayout->addWidget(ViewList[i], i / ct, i % ct, 1, 1);
        }
    }
    else if (num < ViewList.size()) // 删除
    {
        // 清空布局
        for (int i = 0; i < ViewList.size(); i++)
        {
            DispLayout->removeWidget(ViewList[i]);
        }
        // 删除最后一个
        int Rnum = ViewList.size() - num;
        for (int i = 0; i < Rnum; i++)
        {
            disconnect(ViewList[ViewList.size() - 1], &AlgoGraphicView::Expand, this, &AlgoGraphicViewImageViewList::Expand);

            disconnect(ViewList[ViewList.size() - 1], &AlgoGraphicView::newNavPointSignal, this, &AlgoGraphicViewImageViewList::onSetNewNavPoint);

            delete ViewList[ViewList.size() - 1];
            ViewList.removeAt(ViewList.size() - 1);
        }
        // 重新布局
        int count = ViewList.size();
        int ct = count <= 4 ? 2 : (count <= 6 ? 3 : 4); // 动态调整布局
        for (int i = 0; i < ViewList.size(); i++)
        {
            DispLayout->addWidget(ViewList[i], i / ct, i % ct, 1, 1);
        }
    }
}

void AlgoGraphicViewImageViewList::onSetNewNavPoint(QPointF point)
{
    qDebug() << "AlgoGraphicViewImageViewList::onSetNewNavPoint: " << point;
}

// 单独放大某一个窗口 使用隐藏的方式、不显示的隐藏后布局自适应。
void AlgoGraphicViewImageViewList::Expand(QWidget *ptr)
{
    if (Ex)
    {
        for (auto item : ViewList)
        {
            item->setVisible(true);
        }
        Ex = false;
    }
    else
    {
        AlgoGraphicView *tmp = (AlgoGraphicView *)ptr;
        for (auto item : ViewList)
        {
            if (item != tmp)
            {
                item->setVisible(false);
            }
        }
        Ex = true;
    }
}
// 显示信息
void AlgoGraphicViewImageViewList::DispInfo(int CamNum, int statue, int Result, QString str, bool All)
{
    if (CamNum < 1 || CamNum > ViewList.size())
        return;

    if (All)
    {
        ViewList[CamNum - 1]->DispInfo(statue); // 相机连接状态
    }
    else
    {
        ViewList[CamNum - 1]->DispInfo(statue, Result, str, All); // 相机连接状态
    }
}
// 显示图片
void AlgoGraphicViewImageViewList::DispImage(int CamNum, QImage &img)
{
    if (CamNum < 1 || CamNum > ViewList.size())
        return;
    ViewList[CamNum - 1]->DispImage(img);
}

void AlgoGraphicViewImageViewList::DispPoint(int CamNum, QPointF point, double len, double angel, QColor color)
{
    if (CamNum < 1 || CamNum > ViewList.size())
        return;

    QVector<QPointF> pointList;
    QPointF paramPoint;

    paramPoint.setX(len);
    paramPoint.setY(angel);

    pointList << point << paramPoint;

    ViewList[CamNum - 1]->DispPoint(pointList, color);
}

void AlgoGraphicViewImageViewList::DispLine(int CamNum, QVector<QPointF> &list, QColor color)
{
    if (CamNum < 1 || CamNum > ViewList.size())
        return;
    ViewList[CamNum - 1]->DispLine(list, color);
}

void AlgoGraphicViewImageViewList::ResetRotRect(int CamNum)
{
    if (CamNum < 1 || CamNum > ViewList.size())
        return;
    ViewList[CamNum - 1]->ResetRotRect();
}

void AlgoGraphicViewImageViewList::ResetCircle(int CamNum)
{
    if (CamNum < 1 || CamNum > ViewList.size())
        return;
    ViewList[CamNum - 1]->ResetCircle();
}

void AlgoGraphicViewImageViewList::ResetText(int CamNum)
{
    if (CamNum < 1 || CamNum > ViewList.size())
        return;
    ViewList[CamNum - 1]->ResetText();
}

void AlgoGraphicViewImageViewList::DispRotRect(int CamNum, double x, double y, double angel, double width, double height)
{
    if (CamNum < 1 || CamNum > ViewList.size())
        return;
    ViewList[CamNum - 1]->DispRotRect(x, y, angel, width, height);
}

void AlgoGraphicViewImageViewList::DispCircle(int CamNum, double x, double y, double radius)
{
    if (CamNum < 1 || CamNum > ViewList.size())
        return;
    ViewList[CamNum - 1]->DispCircle(x, y, radius);
}

void AlgoGraphicViewImageViewList::DispText(int CamNum, double x, double y, QString text, int size, QColor color)
{
    if (CamNum < 1 || CamNum > ViewList.size())
        return;
    ViewList[CamNum - 1]->DispText(x, y, text, size, color);
}

void AlgoGraphicViewImageViewList::SetToFit(int CamNum)
{
    if (CamNum < 1 || CamNum > ViewList.size())
        return;
    ViewList[CamNum - 1]->SetToFit();
}
void AlgoGraphicViewImageViewList::ClearObj(int CamNum)
{
    if (CamNum < 1 || CamNum > ViewList.size())
        return;
    ViewList[CamNum - 1]->ClearObj();
}

void AlgoGraphicViewImageViewList::ClearPoint(int CamNum)
{
    if (CamNum < 1 || CamNum > ViewList.size())
        return;
    ViewList[CamNum - 1]->ClearAll();
}

QPointF AlgoGraphicViewImageViewList::getCurrentViewCenter(int CamNum)
{
    if (CamNum < 1 || CamNum > ViewList.size())
        return QPointF(0, 0);
    return ViewList[CamNum - 1]->getCurrentViewCenter();
}

void AlgoGraphicViewImageViewList::deleteLastItem(int CamNum)
{
    if (CamNum < 1 || CamNum > ViewList.size())
        return;
    ViewList[CamNum - 1]->deleteLastItem();
}

QStack<AlgoGraphicViewBaseItem *> AlgoGraphicViewImageViewList::getRoiItemStack(int CamNum)
{
    if (CamNum < 1 || CamNum > ViewList.size())
        return QStack<AlgoGraphicViewBaseItem *>();
    return ViewList[CamNum - 1]->getRoiItemStack();
}

void AlgoGraphicViewImageViewList::AddItems(int CamNum, AlgoGraphicViewBaseItem *item)
{
    if (CamNum < 1 || CamNum > ViewList.size())
        return;

    ViewList[CamNum - 1]->AddItems(item);
}