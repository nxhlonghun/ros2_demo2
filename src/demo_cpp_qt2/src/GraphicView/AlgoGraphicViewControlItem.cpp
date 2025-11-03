#include "AlgoGraphicViewControlItem.h"
#include "AlgoGraphicViewBaseItem.h"
#include <QDebug>




//构造函数
AlgoGraphicViewControlItem::AlgoGraphicViewControlItem(QGraphicsItemGroup* parent, QPointF p, int type) : QAbstractGraphicsShapeItem(parent)
  ,point(p),pointType(type)
{
    this->setPos(point);
    if(pointType==0)
    {
        this->setCursor(Qt::OpenHandCursor);
        //this->setParentItem(parent);//需要测试
    }else
    {
        this->setCursor(Qt::PointingHandCursor);
        this->setVisible(false);
    }
    this->setBrush(QBrush(QColor(0, 100, 250)));//控制点颜色
    setCacheMode(DeviceCoordinateCache);//设置缓冲 需要配合 update 使用

    //非常关键 确保点击子元素不会丢失焦点
    this->setFlags(QGraphicsItem::ItemIsSelectable |
                   QGraphicsItem::ItemIsMovable |
                   ItemStopsFocusHandling);

    MQRect=QRectF(-AlgoGraphicViewBaseItem::ContrSize, -AlgoGraphicViewBaseItem::ContrSize, AlgoGraphicViewBaseItem::ContrSize*2, AlgoGraphicViewBaseItem::ContrSize*2);
    myPen.setWidthF(0);//缩放后线宽 0 为任何情况下1像素
}

QPointF AlgoGraphicViewControlItem::GetPoint()
{
    return point;
}
void AlgoGraphicViewControlItem::SetPoint(QPointF p)
{
    point=p;
    this->update();
}


//拖拽 鼠标感应区域
QRectF AlgoGraphicViewControlItem::boundingRect() const
{
    return MQRect; //拖拽 鼠标感应区域
}
//位置重绘
void AlgoGraphicViewControlItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //位置重绘
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(myPen);
    painter->setBrush(this->brush());  
    this->setPos(point);

    if(pointType==0)//0号点默认为中心点
    {
        painter->drawRect(MQRect);//拖拽 显示框
    }else
    {   
        //浮点型椭圆、整数椭圆缩放时会出问题
        painter-> drawEllipse(MQRect);
    }
}
//鼠标事件处理
void AlgoGraphicViewControlItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if ( event->buttons() == Qt::LeftButton )
    {

        dx = event->scenePos().x() - event->lastScenePos().x();
        dy = event->scenePos().y() - event->lastScenePos().y();

        AlgoGraphicViewBaseItem* item = static_cast<AlgoGraphicViewBaseItem *>(this->parentItem());

        if(pointType==0)
        {
            item->moveBy(dx, dy);
        }else
        {
            //记录上一次座标结果
            BefPoint=point;
            point = this->mapToParent( event->pos() );
            //更新结果
            bool flg=item->UpDate(pointType);
            if(flg)
            {
                //结果正常、更新位置
                this->setPos(point);
            }else
            {
                //结果异常、退回上一次的位置
                point=BefPoint;
                this->setPos(point);
            }
        }

    }

	event->accept();
}

void AlgoGraphicViewControlItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    if(pointType==0)
    {
        //点击中心点时、激活编辑
       AlgoGraphicViewBaseItem* item = static_cast<AlgoGraphicViewBaseItem *>(this->parentItem());
       item->setFocus();
    }

    QGraphicsItem::mousePressEvent(event);

	event->accept();
}

