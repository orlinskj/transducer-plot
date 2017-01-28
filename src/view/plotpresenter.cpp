#include "plotpresenter.h"

#include <algorithm>
#include <sstream>

#include <QGraphicsLayout>
#include <QBoxLayout>
#include <QStyle>
#include <QLineSeries>
#include <QValueAxis>
#include <QLogValueAxis>

ac::PlotPresenter::PlotPresenter(QObject* object)
{
    QWidget* widget = dynamic_cast<QWidget*>(object);
    if (widget && widget->layout())
    {
        view_ = new QChartView();
        widget->layout()->addWidget(view_);
    }
    else
        view_ = new QChartView(widget);

    broom_ = nullptr;

    view_->setRenderHint(QPainter::Antialiasing);
    view_->setContentsMargins(0,0,0,0);
    view_->setDragMode(QGraphicsView::ScrollHandDrag);
}

ac::PlotPresenter::~PlotPresenter()
{
    /// TODO this might cause problems
    view_->setChart(nullptr);
}

void ac::PlotPresenter::show_plot(ac::Plot *plot)
{
    // looking for plot in internal storage
    auto it = std::find_if(plots_.cbegin(), plots_.cend(),
                 [plot](const ac::PlotAdapterPtr& plot_ptr){ return plot_ptr->plot() == plot; });
    if (it != plots_.end())
        view_->setChart((*it)->chart());
    else
    {
        plots_.emplace_back(ac::PlotAdapterPtr(new ac::PlotAdapter(plot)));
        view_->setChart(plots_.back()->chart());
    }

    if (broom_)
        delete broom_;
    broom_ = new ac::Broom(view_->chart());
    broom_->show();
}

void ac::PlotPresenter::set_log_axis()
{
    QList<QAbstractAxis*> axes = view_->chart()->axes(Qt::Vertical);
    auto old_axis = axes.first();

    QLogValueAxis* axisY = new QLogValueAxis;
    axisY->setLabelFormat("%g");
    axisY->setTitleText(old_axis->titleText());
    axisY->setBase(10);

    view_->chart()->addAxis(axisY,Qt::AlignLeft);

    QList<QAbstractSeries*> series = view_->chart()->series();
    for (auto& s : series)
    {
        for ( auto& a : s->attachedAxes())
        {
            if (a == old_axis)
            {
                s->detachAxis(old_axis);
                s->attachAxis(axisY);
            }
        }
    }

    view_->chart()->removeAxis(old_axis);
}

void ac::PlotPresenter::set_linear_axis()
{
    QList<QAbstractAxis*> axes = view_->chart()->axes(Qt::Vertical);
    auto old_axis = axes.first();

    QValueAxis* axisY = new QValueAxis;
    axisY->setTitleText(old_axis->titleText());

    view_->chart()->addAxis(axisY,Qt::AlignLeft);

    QList<QAbstractSeries*> series = view_->chart()->series();
    for (auto& s : series)
    {
        for ( auto& a : s->attachedAxes())
        {
            if (a == old_axis)
            {
                s->detachAxis(old_axis);
                s->attachAxis(axisY);
            }
        }
    }

    view_->chart()->removeAxis(old_axis);
}

QChartView* ac::PlotPresenter::view() const
{
    return view_;
}

ac::Plot* ac::PlotPresenter::plot() const
{
    return plot_;
}

void ac::PlotPresenter::update_plot_cache(Plot *plot)
{
    // looking for plot in internal storage
    auto it = std::find_if(plots_.cbegin(), plots_.cend(),
                 [plot](const ac::PlotAdapterPtr& plot_ptr){ return plot_ptr->plot() == plot; });
    if (it != plots_.end())
        (*it)->update();
    else
        plots_.emplace_back(ac::PlotAdapterPtr(new ac::PlotAdapter(plot)));
}

void ac::PlotPresenter::remove_plot_cache(Plot* plot)
{
    // looking for plot in internal storage
    auto it = std::find_if(plots_.cbegin(), plots_.cend(),
                 [plot](const ac::PlotAdapterPtr& plot_ptr){ return plot_ptr->plot() == plot; });
    if (it != plots_.end())
        plots_.erase(it);
}
