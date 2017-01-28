#ifndef PLOTADAPTER_H
#define PLOTADAPTER_H

#include <QChart>
#include <QLineSeries>
#include "../model/plot.h"
#include "../model/function.h"
#include <memory>

using namespace QtCharts;

namespace ac{

class PlotAdapter
{
public:
    PlotAdapter(Plot* plot=nullptr);
    void update();
    Plot* plot() const;
    QChart* chart() const;

    QLineSeries* series(const Function* func) const;

protected:
    QChart* chart_;
    Plot* plot_;

};

using PlotAdapterPtr = std::unique_ptr<PlotAdapter>;

}

#endif // PLOTADAPTER_H
