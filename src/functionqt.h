#ifndef QTFUNCTION_H
#define QTFUNCTION_H

#include <qcustomplot.h>
#include "function.h"
#include "plotqt.h"

namespace ac
{

class PlotQt;

class FunctionQt : public Function
{
public:
    FunctionQt(const DataSet& ds, const Series* domain, const Series* codomain, PlotQt* plot);
    FunctionQt(Function&& f, PlotQt* plot);
    FunctionQt(const Function& f, PlotQt* plot);
    virtual ~FunctionQt();

    virtual void set();
    virtual void unset();

protected:
    PlotQt* plot;
    QCPGraph* graph;
};

}



#endif // QTFUNCTION_H
