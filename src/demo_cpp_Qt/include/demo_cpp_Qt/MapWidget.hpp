#ifndef MAPWIDGET_HPP
#define MAPWIDGET_HPP

#include <QWidget>
#include <QApplication>
#include <QLabel>
#include <QString>
#include <QFile>
#include <QMutex>
#include <QPainter>
#include <QPushButton>
#include "AlgoGraphicViewImageViewList.h"

#include <iostream>
#include <cmath>

class MapWidget : public AlgoGraphicViewImageViewList
{
    Q_OBJECT
public:
    explicit MapWidget(AlgoGraphicViewImageViewList *parent = nullptr, QPushButton *button = nullptr);
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
    void getLocalPathUpdated(const QVector<QPointF> &points) { onLocalPathUpdated(points); }
    void getGlobalPathUpdated(const QVector<QPointF> &points) { onGlobalPathUpdated(points); }
    void getMapUpdated(const QVector<int> &data, unsigned int width, unsigned int height,
                       double resolution, double origin_x, double origin_y, double origin_theta)
    {
        onMapUpdated(data, width, height, resolution, origin_x, origin_y, origin_theta);
    }
    void getPushButtonClick(int flog)
    {
        onPushButtonClick(flog);
    }
    // QImage pixmap_;
signals:
    void pixMapUpdate(QImage pixmap_);
    void mousePress(double x, double y, double yaw);

public slots:

    virtual void onSetNewNavPoint(QPointF point) override;

private slots:
    void onPoseUpdated(double x, double y, double yaw);
    void onLaserUpdated(const QVector<QPointF> &points); // points in robot frame
    void onOccupancyLocalGridUpdated(const QVector<int> &data, unsigned int width, unsigned int height,
                                     double resolution, double origin_x, double origin_y, double origin_theta);
    void onOccupancyGlobalGridUpdated(const QVector<int> &data, unsigned int width, unsigned int height,
                                      double resolution, double origin_x, double origin_y, double origin_theta);
    void onLocalPathUpdated(const QVector<QPointF> &points);
    void onGlobalPathUpdated(const QVector<QPointF> &points);
    void onMapUpdated(const QVector<int> &data, unsigned int width, unsigned int height, double resolution, double origin_x, double origin_y, double origin_theta);

    void onPushButtonClick(int flog);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPointF worldToImage(double wx, double wy) const;
    QPointF robotLocalToWorld(double lx, double ly) const;
    QPointF imageToWorld(double px, double py) const;

    int flog_ = 0;

    QImage Map_;
    double map_resolution_{0.05};
    double map_origin_x_{0.0}, map_origin_y_{0.0}, map_origin_theta_{0.0};

    double robot_x_{0.0}, robot_y_{0.0}, robot_yaw_{0.0};

    QVector<QPointF> laser_points_robot_;

    // 导航
    QPushButton *button;
    QPointF Point[2];
    int flog_nav = 0;

    QImage baseMap_;
    double baseMap_resolution_{0.05};
    double baseMap_origin_x_{0.0}, baseMap_origin_y_{0.0}, baseMap_origin_theta_{0.0};

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

    QVector<QPointF> local_path_points_;
    QVector<QPointF> global_path_points_;

    // 建图
    QImage liveMap_;
    double liveMap_resolution_{0.05};
    double liveMap_origin_x_{0.0}, liveMap_origin_y_{0.0}, liveMap_origin_theta_{0.0};

    QVector<int> liveMap_data_;
    unsigned int liveMap_width_{0}, liveMap_height_{0};

    mutable QMutex mutex_;
};

#endif // MAPWIDGET_HPP