#ifndef PLOTPRESENTER_H
#define PLOTPRESENTER_H

#include <QChartView>
#include <QObject>

using namespace QtCharts;

#include "../model/plot.h"

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
public slots:
    void set_log_axis();
    void set_linear_axis();

protected:
    QChartView* view_;
};

}

#endif // PLOTPRESENTER_H
