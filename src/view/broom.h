#ifndef BROOM_H
#define BROOM_H

#include <QGraphicsItem>
#include <QChart>

#include "../viewmodel/plotitem.h"

using namespace QtCharts;


class Broom : public QGraphicsItem
{
public:
    Broom(QGraphicsItem *parent = nullptr);

    void set_position(double pos);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);

    QChart* chart() const;
    PlotItem* plot() const;

    void set_plot(PlotItem* plot_adapter);

    void update();

protected:
    //void mousePressEvent(QGraphicsSceneMouseEvent *event);
    //void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    PlotItem* plot_;
    QRectF box_;

private:
    static constexpr int    box_offset_ = 15;
    static constexpr int    marker_size_ = 8;
    static constexpr int    marker_font_size_ = 10;
    static constexpr int    marker_entry_height_ = marker_font_size_ + 5;
};


#endif // BROOM_H
