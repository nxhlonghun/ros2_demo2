#ifndef ALGO_GRAPHIC_SCENE
#define ALGO_GRAPHIC_SCENE

#include <QGraphicsScene>

class AlgoGraphicViewScene : public QGraphicsScene
{
    Q_OBJECT

public:
    AlgoGraphicViewScene(QObject *parent = nullptr);

    void startCreate();

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

signals:
    void updatePoint(QPointF p, QList<QPointF> list, bool isCenter);
    void createFinished();
    void emitMouse(double x, double y);

protected:
    QList<QPointF> Plist;
    bool PolygonFlg;
};

#endif