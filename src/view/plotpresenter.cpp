#include "plotpresenter.h"

#include <QGraphicsLayout>
#include <QBoxLayout>

ac::PlotPresenter::PlotPresenter(QWidget* widget)
{
    if (widget && widget->layout())
    {
        view_ = new QChartView();
        widget->layout()->addWidget(view_);
    }
    else
    {
        view_ = new QChartView(widget);
    }

    view_->viewport()->setAcceptDrops(true);
    view_->chart()->layout()->setContentsMargins(0,0,0,0);

    view_->setRenderHint(QPainter::Antialiasing);
    view_->setContentsMargins(0,0,0,0);
}

void ac::PlotPresenter::show_plot(Plot_ptr plot)
{

}
