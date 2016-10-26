#ifndef PLOTQT_H
#define PLOTQT_H

#include <memory>
#include <qcustomplot.h>
#include "plot.h"
#include "functionqt.h"

namespace ac {

class PlotQt : public Plot
{
public:
    PlotQt(QCustomPlot* p);

    void addFunction(const Function& f);
    QCustomPlot* getPlot();

private:

    QCustomPlot* plot;
};

}   // namespace ac

#endif // PLOTQT_H
