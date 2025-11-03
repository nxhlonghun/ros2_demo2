#ifndef ALGO_GRAPHIC_VIEW
#define ALGO_GRAPHIC_VIEW

/************************************************************************
 * @auther         :
 * @date           : 2021-4
 * @description    : 图像显示模块
 *滚轮控制缩放
 *双击还原最佳状态
 * 左键拖动
 * 可以添加ROI
 * ROI的线条不随缩放发生变化
 ************************************************************************/

#include <QGraphicsView>
#include <QStack>
#include <QMutex>
#include "AlgoGraphicViewBaseItem.h"

class QLabel;
class QMouseEvent;
class AlgoGraphicViewScene;
class AlgoGraphicViewImageItem;
class AlgoGraphicView : public QGraphicsView
{
    Q_OBJECT
public:
    AlgoGraphicView(QWidget *parent = nullptr);
    void SetToFit();
    void ClearObj();
    QPointF getCurrentViewCenter();
    void deleteLastItem();

    AlgoGraphicViewScene *scene;
    DrawItem *drawItem;
    QImage ImageCatch;

    QStack<AlgoGraphicViewBaseItem *> getRoiItemStack() { return m_itemStack; }

public slots:
    void DispImage(QImage &Image);                                            // 显示图片
    void DispPoint(QVector<QPointF> &list, QColor color = QColor(0, 255, 0)); // 显示点集合 默认颜色绿色
    void DispRotRect(double x, double y, double angel, double width, double height);
    void DispCircle(double x, double y, double radius);
    void DispText(double x, double y, QString text, int size, QColor color = QColor(255, 0, 0));
    void AddItems(AlgoGraphicViewBaseItem *item);                                  // 添加ROI
    void DispInfo(int statue, int Result = 0, QString str = "", bool All = false); // 显示信息
    void SetType(bool InfoFlg, bool GrayValueFlg);                                 // 设定显示信息  设定灰度值显示状态（未完成）
    void ClearAll();
    void ClearPoint();
    void DispLine(QVector<QPointF> &list, QColor color = QColor(0, 255, 0)); // 显示点集合 默认颜色绿色
    void ResetRotRect();
    void ResetCircle();
    void ResetText();

    void onOpenImage();
    void onSaveImage();
    void onClearImageCount();
    void onMoveToCurPos();
    void onShowHideCenterCross();

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void drawBackground(QPainter *painter, const QRectF &rect) override;
    void updateSceneRect();

private:
    void ZoomFrame(double value);
    void GetFit();
    double ZoomValue = 1;
    double ZoomFit = 1;
    double PixX = 0;
    double PixY = 0;
    QPointF m_curMousePos;
    QPixmap image;
    AlgoGraphicViewImageItem *item;
    QWidget *StatueLight;
    QWidget *StatueInfo;
    QWidget *fWidget;
    QLabel *NGLab;
    QLabel *OKLab;
    QLabel *GrayValue; // 显示灰度值

    QStack<AlgoGraphicViewBaseItem *> m_itemStack;

    QMutex m_mutex;
    unsigned long m_imgIndex = 0;

    QAction *m_showCenterAction = nullptr;

    int m_rectItemIndex = 0;
    QList<QGraphicsRectItem *> m_rectItemsList;

    int m_circleItemIndex = 0;
    QList<QGraphicsEllipseItem *> m_circleItemsList;

    int m_textItemIndex = 0;
    QList<QGraphicsTextItem *> m_textItemsList;

private:
    int MRSize = 8;                                // 拖拽区域大小
    QColor color1 = QColor(128, 128, 128);         // 颜色1
    QColor color2 = QColor(0xc9, 0xca, 0xcc, 150); // 颜色9cacc
    QPixmap bgPix = QPixmap(64, 64);               // 背景图片
    void drawBg();
signals:
    void Expand(QWidget *Prt);
    void moveToPosSignal(QPointF point);
    void mouseClicked();
};

#endif
