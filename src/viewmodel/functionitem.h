#ifndef FUNCTIONITEM_H
#define FUNCTIONITEM_H

#include <QLineSeries>


using namespace QtCharts;

#include "../model/function.h"
#include "../viewmodel/treemodel/treeitemt.h"

class PlotItem;

class FunctionItem : public TreeItemT<Function>
{
public:
    FunctionItem(Function *f = nullptr);

    QLineSeries* series() const;
    QPointF top() const;
    QPointF bottom() const;
    QPointF right() const;
    QPointF left() const;

protected:
    std::unique_ptr<QLineSeries> series_;
    QPointF top_, bottom_, right_, left_;

};

#endif // FUNCTIONITEM_H
