#ifndef PLOTPRESENTER_H
#define PLOTPRESENTER_H

#include <QChartView>
#include <QObject>
#include <QLineSeries>
#include <vector>
#include <memory>

using namespace QtCharts;

#include "../model/plot.h"
#include "../model/function.h"
#include "plotadapter.h"
#include "broom.h"

namespace ac{

class PlotPresenter : public QObject
{
    Q_OBJECT
public:
    // widget should have added some layout to expand the chart view
    PlotPresenter(QObject *object = nullptr);
    ~PlotPresenter();

    void show_plot(Plot* plot);
    QChartView* view() const;
    Plot* plot() const;

    QLineSeries* series_from_func(const ac::Function* func);
    QChart* default_chart() const;

public slots:
    void set_log_axis();
    void set_linear_axis();
    void update_plot_cache(Plot* plot);
    void remove_plot_cache(Plot* plot);

protected:
    QChartView* view_;
    Plot* plot_;
    std::vector<PlotAdapterPtr> plots_;
    Broom* broom_;
};

}

#endif // PLOTPRESENTER_H
