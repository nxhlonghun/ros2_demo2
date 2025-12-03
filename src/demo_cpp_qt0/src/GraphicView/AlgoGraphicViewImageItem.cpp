#include "AlgoGraphicViewImageItem.h"
#include <QDebug>
#include <QGraphicsSceneHoverEvent>

AlgoGraphicViewImageItem::AlgoGraphicViewImageItem(QWidget *parent) : QGraphicsPixmapItem(nullptr)
{

}

void AlgoGraphicViewImageItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QPointF mousePosition = event->pos();
    int R,G,B;
    pixmap().toImage().pixelColor(mousePosition.x(), mousePosition.y()).getRgb(&R,&G,&B);

    QString InfoVal=QString("[%1 %2 %3]\n{X%4 Y%5}").arg(QString::number(R),3,' ')
            .arg(QString::number(G),3,' ')
            .arg(QString::number(B),3,' ')
            .arg(QString::number((int)mousePosition.x()),4,' ')
            .arg(QString::number((int)mousePosition.y()),4,' ');

    emit RGBValue(InfoVal, mousePosition);
}



