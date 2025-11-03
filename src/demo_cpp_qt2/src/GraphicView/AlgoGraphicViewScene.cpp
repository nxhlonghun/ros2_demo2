#include "AlgoGraphicViewScene.h"
#include <QGraphicsSceneMouseEvent>


AlgoGraphicViewScene::AlgoGraphicViewScene(QObject *parent) : QGraphicsScene(parent)
{
    PolygonFlg = false;
}

void AlgoGraphicViewScene::startCreate()
{
    PolygonFlg = true;
    Plist.clear();
}

void AlgoGraphicViewScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (PolygonFlg)
    {
        QPointF p(event->scenePos().x(), event->scenePos().y());

        switch ( event->buttons() )
        {
        case Qt::LeftButton: {
            Plist.push_back(p);
            emit updatePoint(p, Plist, false);
        } break;
        case Qt::RightButton: {
            if (Plist.size() >= 3) {
                emit updatePoint(p, Plist, true);
                emit createFinished();
                PolygonFlg = false;
                Plist.clear();
            }
        } break;
        default: break;
        }
    } else {
        QGraphicsScene::mousePressEvent(event);
    }

	event->accept();
}

