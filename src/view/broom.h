#ifndef BROOM_H
#define BROOM_H

#include <QGraphicsItem>
#include <QChart>

#include "../viewmodel/plotitem.h"

using namespace QtCharts;

/**
 * @brief The Broom class
 *
 * Moveable graphical item. Displays function values from current x position on the chart domain.
 */
class Broom : public QGraphicsItem
{
public:
    Broom(QGraphicsItem *parent = nullptr);

    void set_position(QPointF pos, bool force = false);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
    void resizeEvent(QResizeEvent* event);

    QChart* chart() const;
    PlotItem* plot() const;

    void update_bounding_rect();
    void update_position();
    void set_plot(PlotItem* plot_adapter);
    void toggle();
    void set_visibility(bool visibility);

    void update();

protected:

    double x_value_;
    PlotItem* plot_;
    QRectF box_;
    bool enabled_;

    QRectF bounding_rect_;

private:
    static constexpr int    box_offset_ = 15;
    static constexpr int    marker_size_ = 8;
    static constexpr int    marker_font_size_ = 10;
    static constexpr int    marker_entry_height_ = marker_font_size_ + 5;
};


#endif // BROOM_H
