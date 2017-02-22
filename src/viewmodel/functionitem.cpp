#include "functionitem.h"

#include <QValueAxis>
#include <QLineSeries>

FunctionItem::FunctionItem(Function *f) :
    TreeItemT<Function>(f), series_(new QLineSeries)
{
    // initialize series with values from function
    series_->setName(f->codomain()->unit().name().c_str());
    auto dv = f->domain()->values();
    auto cv = f->codomain()->values();

    int point_count = std::min(dv.size(), cv.size());
    for(int i=0; i<point_count; i++)
        series_->append(dv.at(i),cv.at(i));
}

QLineSeries* FunctionItem::series() const
{
    return series_;
}
