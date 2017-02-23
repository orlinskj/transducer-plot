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
#include "../viewmodel/plotitem.h"
#include "broom.h"

// namespace ac{

class PlotStoreItemModel;

class PlotPresenter : public QGraphicsView
{
    Q_OBJECT
public:
    // widget should have added some layout to expand the chart view
    PlotPresenter(PlotStoreItemModel* store = nullptr);
    ~PlotPresenter();

    void show_plot(PlotItem* plot);

    QChart* chart() const;
    PlotItem* plot() const;

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);

    void alter_menu();

public slots:
    void context_menu(const QPoint& point);

protected:
    PlotItem* plot_;
    PlotStoreItemModel* store_;
    Broom* broom_;

    QMenu menu_;

    // dragging chart
    QPointF drag_start_;
};

// } // namespace ac

#endif // PLOTPRESENTER_H
