#ifndef PLOTPRESENTER_H
#define PLOTPRESENTER_H

#include <QChartView>
#include <QObject>
#include <QLineSeries>
#include <QMenu>

#include <vector>
#include <memory>

using namespace QtCharts;

#include "../model/plot.h"
#include "../model/function.h"
#include "plotadapter.h"
#include "broom.h"

// namespace ac{

class PlotPresenter : public QGraphicsView
{
    Q_OBJECT
public:
    // widget should have added some layout to expand the chart view
    PlotPresenter(QWidget *object = nullptr);
    ~PlotPresenter();

    void show_plot(Plot* plot);
    Plot* plot() const;

    //QAbstractAxis* axis_from_point(QPoint point);

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);

    std::vector<QAbstractAxis*> get_y_axes() const;

public slots:
    void set_log_axis();
    void set_linear_axis();

    void update_plot_cache(Plot* plot);
    void remove_plot_cache(Plot* plot);

    void change_left_axis();
    void change_right_axis();

    void context_menu(const QPoint& point);

protected:
    QChart* chart_;
    Plot* plot_;
    std::vector<PlotAdapterPtr> plots_;
    //Broom* broom_;
    QGraphicsLineItem* broom_line_;
    QGraphicsItemGroup* broom_group_;
    QGraphicsRectItem* broom_rect_;
    QGraphicsSimpleTextItem* broom_text_;

    QMenu menu_;

    // dragging chart
    QPointF drag_start_;
};

// } // namespace ac

#endif // PLOTPRESENTER_H
