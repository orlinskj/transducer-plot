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
#include "admitanceringlayer.h"

// namespace ac{

class PlotStoreItemModel;

/**
 * @brief The PlotPresenter class
 * @desc Widget which displays on the viewport one plot at a time.
 */
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
    PlotStoreItemModel* store();

    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    void alter_menu();
    void alter_axes();
    QImage screenshot(int width, int height);

signals:
    void plot_showed(PlotItem* plot, PlotItem* prev);

public slots:
    void context_menu(const QPoint& point);
    void plot_removed(PlotItem* plot);
    void plot_changed(PlotItem* plot);

protected:
    PlotItem* plot_;
    PlotStoreItemModel* store_;
    Broom* broom_;

    QMenu menu_;

    // dragging chart
    bool drag_enabled_;
    QPointF drag_pos_;

    // zooming chart
    bool zoom_enabled_;
    QPointF zoom_pos_;
    bool zoom_horizontal_;

    // broom control
    bool broom_click_;
    QPointF broom_click_pos_;

    // options
    //bool same_scale_on_axes_;
};

// } // namespace ac

#endif // PLOTPRESENTER_H
