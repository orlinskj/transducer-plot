#ifndef PLOT_H
#define PLOT_H

#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <tuple>
#include <memory>
#include "transducer.h"
#include "function.h"

namespace ac
{

class Function;

using namespace std;

// Representation of plot.
// One plot can have multiple datasets binded to it.
// For every dataset, one function can be attached.
// Attached function is displayed on plot.
class Plot
{
public:
    Plot();
    Plot(const Plot& p);
    Plot(QCustomPlot* p);
    virtual DataSet* addDataSet(DataSet &&dataset);
    virtual void addFunction(const Function& f);
    QCustomPlot* getPlot();

protected:
    vector<DataSet> sets;
    vector<unique_ptr<Function>> functions;
    string name;

    QCustomPlot* plot;
};

}


#endif // PLOT_H
