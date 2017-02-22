#ifndef FUNCTIONITEM_H
#define FUNCTIONITEM_H

//#include <QAbstractSeries>
#include <QLineSeries>

using namespace QtCharts;

#include "../model/function.h"
#include "../viewmodel/treemodel/treeitemt.h"

class FunctionItem : public TreeItemT<Function>
{
public:
    FunctionItem(Function *f = nullptr);

    QLineSeries* series() const;

protected:
    QLineSeries* series_;

};

#endif // FUNCTIONITEM_H
