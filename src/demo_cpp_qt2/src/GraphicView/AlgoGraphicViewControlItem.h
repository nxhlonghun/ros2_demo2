#ifndef ALGO_GRAPHIC_CTRL_ITEM
#define ALGO_GRAPHIC_CTRL_ITEM

/************************************************************************
 * @auther         :
 * @date           : 2021-4
 * @description    : 控制点
 * ROI模块中美的控制点
 * 中心点ju'xin矩形 点击后可移动
 * 控制点圆形、点击后可以拖动
 ************************************************************************/

#include <QObject>
#include <QAbstractGraphicsShapeItem>
#include <QPointF>
#include <QPen>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QCursor>
#include <QKeyEvent>
#include <QList>
#include <QDebug>

class AlgoGraphicViewControlItem : public QObject, public QAbstractGraphicsShapeItem
{
    Q_OBJECT
public:
    explicit AlgoGraphicViewControlItem(QGraphicsItemGroup *parent, QPointF p, int type);
    QPointF GetPoint();
    QPointF GetBefPoint() { return BefPoint; }
    void SetPoint(QPointF p);
    qreal dX() { return dx; };
    qreal dY() { return dy; };

    int index = 0;

protected:
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QPen myPen = this->pen();
    QPointF point;
    int pointType;
    QRectF MQRect; // 控制点巨型框
    qreal dx;
    qreal dy;
    QPointF BefPoint;
};
#endif