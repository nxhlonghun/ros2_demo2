#ifndef ALGO_GRAPHIC_VIEW_BASE_ITEM
#define ALGO_GRAPHIC_VIEW_BASE_ITEM

#include <QJsonObject>

/************************************************************************
 * @auther         :
 * @date           : 2021-4
 * @description    : ROI模块
 *基类模块和各种子类
 *
 ************************************************************************/

struct MRectangle
{
    MRectangle()
    {
    }
    MRectangle(float x, float y, float Width, float Height)
    {
        rows = y;
        cols = x;
        width = Width;
        height = Height;
    }
    float rows;
    float cols;
    float width;
    float height;
};

#include <QObject>
#include "AlgoGraphicViewControlItem.h"
#include <QList>

//
//**************************************基类***************************************************
class AlgoGraphicViewBaseItem : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
public:
    enum ItemType
    {
        AutoItem = 0,        // 自定义类型
        Circle,              // 圆
        Ellipse,             // 椭圆
        Concentric_Circle,   // 同心圆
        Pie,                 // 饼
        Chord,               // 和弦
        Rectangle,           // 矩形
        RectangleR,          // 矩形
        Square,              // 正方形
        Polygon,             // 多边形
        Round_End_Rectangle, // 圆端矩形
        Rounded_Rectangle,   // 圆角矩形
        LineObj              // 圆角矩形
    };
    void SetIndex(int num);
    virtual bool UpDate(int index) { return true; };
    static void SetScale(double value);

    static bool isShowCenter;
    static QPointF centerPoint;
    static double centerPointLen;
    static QColor centerPointColor;

    static void setCenterPoint(QPointF point, double len = 100, QColor color = Qt::red);
    static void showCenterPoint(bool value);

    QList<AlgoGraphicViewControlItem *> ControlList;
    qreal *scale;

    qreal scaler;

    static qreal ContrSize;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    void setItemDiscrib(QString value) { ItemDiscrib = value; };

    ItemType getItemType() { return types; }

    virtual void loadFromJson(QJsonObject value) {};

    virtual QJsonObject writeToJson() { return QJsonObject(); };

protected:
    AlgoGraphicViewBaseItem(QPointF center, ItemType type);
    virtual void focusInEvent(QFocusEvent *event) override;
    virtual void focusOutEvent(QFocusEvent *event) override;

protected:
    QPointF center;
    ItemType types;
    QPen isSelected;
    QPen noSelected;
    QColor fillColor;
    QPen thisPen;
    qreal LineWidth = 2.5; // 控制点初始尺寸
    QString ItemDiscrib = QString::fromLocal8Bit("");
};
//**************************************绘制结果数据***************************************************
class DrawItem : public AlgoGraphicViewBaseItem
{
    Q_OBJECT
public:
    DrawItem();
    void AddLine(QVector<QPointF> &Plist) { LineList.push_back(Plist); };
    void AddRegion(QVector<QPointF> &Plist);
    void AddPoint(QVector<QPointF> &Plist, QColor color = QColor(0, 255, 0));
    void ClearAll();
    void ClearPoint();
    void Refresh();
    void SetDispROI(QRectF &Rec) { returnRec = Rec; };

protected:
    int type() const override { return 100; };
    bool UpDate(int index) { return true; };
    virtual QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QPen DrawPen;
    QRectF returnRec;
    QVector<QVector<QPointF>> LineList;
    QVector<QVector<QPointF>> ROIList;
    QVector<QVector<QPointF>> PointList;
    QVector<QColor> PointColorList;
};
//**************************************矩形*******************************************************
class RectangleItem : public AlgoGraphicViewBaseItem
{
    Q_OBJECT
public:
    RectangleItem(qreal x, qreal y, qreal width, qreal height);

    void SetRect(MRectangle Rec);
    void GetRect(MRectangle &Rec);

    virtual QJsonObject writeToJson() override;

protected:
    virtual QRectF boundingRect() const override;
    bool UpDate(int index) override;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
};
//**************************************矩形 旋转*******************************************************
class RectangleRItem : public AlgoGraphicViewBaseItem
{
    Q_OBJECT
public:
    RectangleRItem(qreal x, qreal y, qreal Lenth1, qreal Lenth2, qreal Pi);

    virtual QJsonObject writeToJson() override;

protected:
    virtual QRectF boundingRect() const override;
    bool UpDate(int index) override;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

protected:
    qreal angle = 0;
    qreal lenth1 = 0;
    qreal lenth2 = 0;

    QPointF Pa1;
    QPointF Pa2;
    QPointF Pa3;
    QPointF Pa4;

    QPointF PArrow;
};
//**************************************椭圆*******************************************************
class EllipseItem : public RectangleRItem
{
    Q_OBJECT
public:
    EllipseItem(qreal x, qreal y, qreal Lenth1, qreal Lenth2, qreal Pi);

protected:
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
};
//**************************************圆*******************************************************
class CircleItem : public AlgoGraphicViewBaseItem
{
    Q_OBJECT
public:
    CircleItem(qreal x, qreal y, qreal R);

protected:
    virtual QRectF boundingRect() const override;
    bool UpDate(int index) override;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

    virtual QJsonObject writeToJson() override;
    qreal Radius;
};
//**************************************同心圆*********************************************************
class ConcentricCircleItem : public AlgoGraphicViewBaseItem
{
    Q_OBJECT
public:
    ConcentricCircleItem(qreal x, qreal y, qreal RadiusMin, qreal RadiusMax);

    virtual QJsonObject writeToJson() override;

protected:
    virtual QRectF boundingRect() const override;
    bool UpDate(int index) override;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
    qreal RadiusMin;
    qreal RadiusMax;
};
//**************************************饼图*********************************************************
class PieItem : public AlgoGraphicViewBaseItem
{
    Q_OBJECT
public:
    PieItem(qreal x, qreal y, qreal radius, qreal startAng, qreal endAng);

protected:
    virtual QRectF boundingRect() const override;
    bool UpDate(int index) override;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
    qreal Radius;
    qreal StartAng;
    qreal EndAng;
};
//**************************************多边形*********************************************************
class PolygonItem : public AlgoGraphicViewBaseItem
{
    Q_OBJECT
public:
    PolygonItem();
    QPointF getCentroid(QList<QPointF> list);
    void getMaxLength();
public slots:
    void pushPoint(QPointF p, QList<QPointF> list, bool isCenter);

protected:
    virtual QRectF boundingRect() const override;
    bool UpDate(int index) override;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

protected:
    qreal Radius;
    bool Finished;
};
//**************************************圆端矩形*********************************************************
class RoundRectangle1Item : public AlgoGraphicViewBaseItem
{
    Q_OBJECT
public:
    RoundRectangle1Item(qreal x, qreal y, qreal width, qreal height);

protected:
    virtual QRectF boundingRect() const override;
    bool UpDate(int index) override;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
};
//**************************************圆角矩形*********************************************************
class RoundRectangle2Item : public AlgoGraphicViewBaseItem
{
    Q_OBJECT
public:
    RoundRectangle2Item(qreal x, qreal y, qreal width, qreal height, qreal radius);

protected:
    virtual QRectF boundingRect() const override;
    bool UpDate(int index) override;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
    qreal Width;
    qreal Height;
    qreal Radius;
};
//**************************************直线*********************************************************
class LineItem : public AlgoGraphicViewBaseItem
{
    Q_OBJECT
public:
    LineItem(qreal x1, qreal y1, qreal x2, qreal y2);

protected:
    bool UpDate(int index) override;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

    QPointF P1;
    QPointF P2;
};
//**************************************卡尺*********************************************************
class Caliper : public AlgoGraphicViewBaseItem
{
    Q_OBJECT
public:
    Caliper(qreal x1, qreal y1, qreal x2, qreal y2, qreal height);
    void GetPosition(qreal &x1, qreal &y1, qreal &x2, qreal &y2, qreal &height);

protected:
    virtual QRectF boundingRect() const override;
    bool UpDate(int index) override;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
    QPointF P1Glob;
    QPointF P2Glob;
    QPointF P1;
    QPointF P2;
    qreal Height;
    qreal angle;
    qreal Lenth;
};

#endif