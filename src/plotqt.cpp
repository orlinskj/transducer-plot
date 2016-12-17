#include "plotqt.h"

using namespace ac;



void PlotQt::addFunction(const Function& f)
{
    functions.push_back(unique_ptr<FunctionQt>(new FunctionQt(f,this)));
}

QCustomPlot* PlotQt::getPlot()
{
    return plot;
}
