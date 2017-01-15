#ifndef PLOTPRESENTER_H
#define PLOTPRESENTER_H

#include <QChartView>

using namespace QtCharts;

#include "../model/plot.h"

namespace ac{

class PlotPresenter
{
public:
    // widget should have added some layout to expand the chart view
    PlotPresenter(QWidget* widget = nullptr);

    void show_plot(Plot* plot);

    QChartView* view_;
};

}

#endif // PLOTPRESENTER_H
