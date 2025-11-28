#ifndef ALGO_GRAPHIC_IMAGE_ITEM
#define ALGO_GRAPHIC_IMAGE_ITEM


/************************************************************************
  * @auther         :
  * @date           : 2021-4
  * @description    : 通过鼠标点选获取当前灰度值
************************************************************************/
#include <QGraphicsPixmapItem>

class AlgoGraphicViewImageItem :public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit AlgoGraphicViewImageItem(QWidget *parent = nullptr);

protected:
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);


signals:
    void RGBValue(QString InfoVal, QPointF mousePos);


};

#endif