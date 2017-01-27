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
    {
        view_ = new QChartView(widget);
    }

    view_->viewport()->setAcceptDrops(true);
    view_->chart()->layout()->setContentsMargins(0,0,0,0);

    view_->setRenderHint(QPainter::Antialiasing);
    view_->setContentsMargins(0,0,0,0);
    view_->setDragMode(QGraphicsView::ScrollHandDrag);
}

ac::PlotPresenter::~PlotPresenter() {}

void ac::PlotPresenter::show_plot(Plot *plot)
{
    for(int i=0; i<plot->children_count(); i++)
    {
        auto func = dynamic_cast<Function*>(plot->child(i));
        QLineSeries* series = new QLineSeries();

        int points_count = std::min(
                    func->get_domain()->get_values().size(),
                    func->get_codomain()->get_values().size());

        for(int v=0; v<points_count; v++)
        {
            series->append(
                        func->get_domain()->get_values().at(v),
                        func->get_codomain()->get_values().at(v));
        }

        QChart *chart = new QChart();
        chart->legend()->hide();
        chart->addSeries(series);
        //chart->createDefaultAxes();
        std::stringstream ss;
        ss << "Wykres " << func->get_domain()->get_unit().get_name()
           << " od " << func->get_codomain()->get_unit().get_name();
        chart->setTitle(ss.str().c_str());

        QValueAxis *axisX = new QValueAxis;
        std::stringstream axis_x_ss;
        axis_x_ss << func->get_domain()->get_unit().get_name()
                  << "[" << func->get_domain()->get_unit().get_symbol() << "]";
        axisX->setTitleText(axis_x_ss.str().c_str());
        //axisX->setTickCount(6);
        axisX->setLabelFormat("%i");
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);

        QLogValueAxis *axisY = new QLogValueAxis;
        axisY->setLabelFormat("%g");
        std::stringstream axis_y_ss;
        axis_x_ss << func->get_codomain()->get_unit().get_name()
                  << "[" << func->get_codomain()->get_unit().get_symbol() << "]";
        axisY->setTitleText(axis_y_ss.str().c_str());
        axisY->setBase(10);
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);

        if (view_->chart())
            delete view_->chart();
        view_->setChart(chart);
    }
}

void ac::PlotPresenter::set_log_axis()
{
    QList<QAbstractAxis*> axes = view_->chart()->axes(Qt::Vertical);
    auto old_axis = axes.first();

    QLogValueAxis* axisY = new QLogValueAxis;
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
