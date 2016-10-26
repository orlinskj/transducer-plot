#ifndef PLOT_H
#define PLOT_H

#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <tuple>
#include <memory>
#include "dataset.h"
#include "function.h"

namespace ac
{

using namespace std;

// Representation of plot.
// One plot can have multiple datasets binded to it.
// For every dataset, one function can be attached.
// Attached function is displayed on plot.
class Plot
{
public:
    virtual void addDataSet(DataSet &&dataset);
    virtual void addFunction(const Function& f);

protected:
    vector<DataSet> sets;
    vector<unique_ptr<Function>> functions;
    string name;

};

}


#endif // PLOT_H
