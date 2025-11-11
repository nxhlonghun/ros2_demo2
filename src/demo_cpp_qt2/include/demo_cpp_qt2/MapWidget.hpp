#ifndef MAPWIDGET_HPP
#define MAPWIDGET_HPP

#include <QWidget>
#include <QApplication>
#include <QLabel>
#include <QString>
#include <QFile>
#include <QMutex>
#include <QPainter>
#include "AlgoGraphicViewImageViewList.h"

#include <iostream>

class MapWidget : public AlgoGraphicViewImageViewList
{
    Q_OBJECT
public:
    explicit MapWidget(AlgoGraphicViewImageViewList *parent = nullptr);
    bool loadMapFromFiles(const QString &pgmFiile, const QString &yamlFile);
    void getPoseUpdated(double x, double y, double yaw) { onPoseUpdated(x, y, yaw); }
    void getLaserUpdated(const QVector<QPointF> &points) { onLaserUpdated(points); }
    void getOccupancyLocalGridUpdated(const QVector<int> &data, unsigned int width, unsigned int height,
                                      double resolution, double origin_x, double origin_y, double origin_theta)
    {
        onOccupancyLocalGridUpdated(data, width, height, resolution, origin_x, origin_y, origin_theta);
    }
    void getOccupancyGlobalGridUpdated(const QVector<int> &data, unsigned int width, unsigned int height,
                                       double resolution, double origin_x, double origin_y, double origin_theta)
    {
        onOccupancyGlobalGridUpdated(data, width, height, resolution, origin_x, origin_y, origin_theta);
    }
    // QImage pixmap_;
signals:
    void pixMapUpdate(QImage pixmap_);

private slots:
    void onPoseUpdated(double x, double y, double yaw);
    void onLaserUpdated(const QVector<QPointF> &points); // points in robot frame
    void onOccupancyLocalGridUpdated(const QVector<int> &data, unsigned int width, unsigned int height,
                                     double resolution, double origin_x, double origin_y, double origin_theta);
    void onOccupancyGlobalGridUpdated(const QVector<int> &data, unsigned int width, unsigned int height,
                                      double resolution, double origin_x, double origin_y, double origin_theta);

protected:
    void paintEvent(QPaintEvent *event) override;
    // void wheelEvent(QWheelEvent *event) override;

private:
    QPoint worldToImage(double wx, double wy) const;
    QPointF robotLocalToWorld(double lx, double ly) const;

    // QGraphicsView *mapView;
    // QGraphicsScene *mapScence;
    // QGraphicsPixmapItem *mapPixmapItem = nullptr;

    // double mapScale_ = 1.0;
    QImage baseMap_;
    double map_resolution_{0.05};
    double map_origin_x_{0.0}, map_origin_y_{0.0}, map_origin_theta_{0.0};

    double robot_x_{0.0}, robot_y_{0.0}, robot_yaw_{0.0};

    QVector<QPointF> laser_points_robot_;

    QVector<int> local_occupancy_data_;
    unsigned int local_occ_width_{0}, local_occ_height_{0};
    double local_occ_resolution_{0};
    double local_occ_origin_x_{0}, local_occ_origin_y_{0}, local_occ_origin_theta_{0};
    QImage local_occ_image_; // 用于渲染的灰度图像

    QVector<int> global_occupancy_data_;
    unsigned int global_occ_width_{0}, global_occ_height_{0};
    double global_occ_resolution_{0};
    double global_occ_origin_x_{0}, global_occ_origin_y_{0}, global_occ_origin_theta_{0};
    QImage global_occ_image_; // 用于渲染的灰度图像
    
    mutable QMutex mutex_;
};

#endif