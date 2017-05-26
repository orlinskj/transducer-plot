#include "functionitem.h"

#include <QValueAxis>
#include <QLineSeries>

FunctionItem::FunctionItem(Function *f) :
    TreeItemT<Function>(f), series_(new QLineSeries)
{
    // OpenGL is not supported well yet - it causes problems with Z ordering,
    // taking screenshot and aliasing
    //series_->setUseOpenGL(true);

    // initialize series with values from function
    series_->setName(f->codomain()->unit().name().c_str());
    auto dv = f->domain()->values();
    auto cv = f->codomain()->values();

    int point_count = std::min(dv.size(), cv.size());

    if (point_count > 0){
        series_->append(dv.at(0),cv.at(0));
        top_ = bottom_ = right_ = left_ = series_->at(0);
    }

    for(int i=1; i<point_count; i++){
        series_->append(dv.at(i),cv.at(i));

        if (series_->at(i).y() > top_.y())
            top_ = series_->at(i);
        else if (series_->at(i).y() < bottom_.y())
            bottom_ = series_->at(i);

        if (series_->at(i).x() > right_.x())
            right_ = series_->at(i);
        else if (series_->at(i).x() < left_.y())
            left_ = series_->at(i);
    }
}

QLineSeries* FunctionItem::series() const
{
    return series_.get();
}

QPointF FunctionItem::top() const { return top_; }
QPointF FunctionItem::bottom() const { return bottom_; }
QPointF FunctionItem::right() const { return right_; }
QPointF FunctionItem::left() const { return left_; }
