#ifndef FUNCTIONITEM_H
#define FUNCTIONITEM_H

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
    std::unique_ptr<QLineSeries> series_;

};

#endif // FUNCTIONITEM_H
