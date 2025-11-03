#ifndef MAPWIDGET_HPP
#define MAPWIDGET_HPP

#include <QWidget>
#include <QApplication>
#include <QLabel>
#include <QString>
#include <QFile>
#include <QMutex>
#include <QPainter>

class MapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MapWidget(QLabel *mapLabel = nullptr, QWidget *parent = nullptr);
    bool loadMapFromFiles(const QString &pgmFiile, const QString &yamlFile);
    void getPoseUpdated(double x, double y, double yaw) { onPoseUpdated(x, y, yaw); }
    void getLaserUpdated(const QVector<QPointF> &points) { onLaserUpdated(points); }
    void getOccupancyGridUpdated(const QVector<int> &data, unsigned int width, unsigned int height,
                                 double resolution, double origin_x, double origin_y, double origin_theta)
    {
        onOccupancyGridUpdated(data, width, height, resolution, origin_x, origin_y, origin_theta);
    }

private slots:
    void onPoseUpdated(double x, double y, double yaw);
    void onLaserUpdated(const QVector<QPointF> &points); // points in robot frame
    void onOccupancyGridUpdated(const QVector<int> &data, unsigned int width, unsigned int height,
                                double resolution, double origin_x, double origin_y, double origin_theta);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPoint worldToImage(double wx, double wy) const;
    QPointF robotLocalToWorld(double lx, double ly) const;

    QLabel *mapLabel;
    QImage baseMap_;
    double map_resolution_{0.05};
    double map_origin_x_{0.0}, map_origin_y_{0.0}, map_origin_theta_{0.0};
    double robot_x_{0.0}, robot_y_{0.0}, robot_yaw_{0.0};
    QVector<QPointF> laser_points_robot_;
    QVector<int> occupancy_data_;
    unsigned int occ_width_{0}, occ_height_{0};
    double occ_resolution_{0};
    double occ_origin_x_{0}, occ_origin_y_{0}, occ_origin_theta_{0};
    QImage occ_image_; // 用于渲染的灰度图像
    mutable QMutex mutex_;
};

#endif