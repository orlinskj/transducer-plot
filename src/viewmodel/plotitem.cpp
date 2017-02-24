#include "plotitem.h"

#include <QValueAxis>
#include <QGraphicsLayout>
#include <QStyleOptionGraphicsItem>
#include <QPainter>

#include <map>
#include <regex>
#include <algorithm>

#include "functionitem.h"
#include "../view/unitaxis.h"

PlotItem::PlotItem() : TreeItemTOwner<Plot>(new Plot), chart_(new QChart)
{ }

PlotItem::PlotItem(Plot *plot) : chart_(new QChart)
{
    chart_->setMaximumSize(QSize(10000,10000));
    chart_->layout()->setContentsMargins(0,0,0,0);
    chart_->setMargins(QMargins(2,5,2,2));
    chart_->setTitle(plot->description().c_str());
}

PlotItem::~PlotItem()
{
    // FunctionItem owns the series
    while(chart_->series().length() > 0)
    {
        chart_->removeSeries(chart_->series().back());
    }
}

QChart* PlotItem::chart() const
{
    return chart_;
}

TreeItem* PlotItem::append(TreeItem* item)
{
    Qt::AlignmentFlag positions[] = { Qt::AlignLeft, Qt::AlignRight };

    auto func_item = dynamic_cast<FunctionItem*>(item);
    // transfering ownership of Function object to Plot
    this->value()->add_function(func_item->value());
    chart_->setTitle(this->value()->name().c_str());
    chart_->addSeries(func_item->series());

    // looking for codomain axes in plot, if absent - create
    auto axes = chart_->axes(Qt::Vertical);
    auto axis_it = std::find_if(axes.cbegin(), axes.cend(), [func_item](auto axis){
        auto unit_axis = UnitAxis::from_qabstractaxis(axis);
        if (unit_axis->match_unit(func_item->value()->codomain()->unit()))
            return true;
        return false;
    });

    QAbstractAxis* axis = nullptr;
    if (axis_it != axes.cend())
    {
        axis = *axis_it;
        UnitAxis* unit_axis = dynamic_cast<UnitValueAxis*>(axis);
        if (!unit_axis)
            unit_axis = dynamic_cast<UnitLogValueAxis*>(axis);

        if (unit_axis)
            unit_axis->add_unit(func_item->value()->codomain()->unit());
        else
            throw std::runtime_error("Chart axis is neither UnitValueAxis nor UnitLogValueAxis");
        axis->setTitleText(unit_axis->name().c_str());

    }
    else
    {
        axis = new UnitValueAxis(func_item->value()->codomain()->unit());
        auto position = positions[axes.size()%2];
        chart_->addAxis(axis,position);
    }
    func_item->series()->attachAxis(axis);

    auto x_axis = chart_->axisX();
    if (!x_axis)
    {
        x_axis = new UnitValueAxis(func_item->value()->domain()->unit());
        chart_->addAxis(x_axis,Qt::AlignBottom);
    }
    func_item->series()->attachAxis(x_axis);

    return TreeItem::append(item);
}

QList<QAbstractSeries*> PlotItem::axis_series(QAbstractAxis* axis)
{
    QList<QAbstractSeries*> list;
    for (auto series : chart_->series())
    {
        if (series->attachedAxes().contains(axis))
            list.append(series);
    }
    return list;
}

void PlotItem::change_axis_alignment(QAbstractAxis* axis, Qt::Alignment align)
{
    auto series_attached = axis_series(axis);
    // this remove axes from all series
    chart_->removeAxis(axis);
    chart_->addAxis(axis, align);
    for (auto series : series_attached)
        series->attachAxis(axis);
}

void PlotItem::remove(TreeItem *item)
{
    // 1. detach axes from series
    // 2. check whether axes from step 1 have any series attached
    // 3. if not - remove those axes from chart
    // 4. remove series from plot

    auto func_item = dynamic_cast<FunctionItem*>(item);

    // looking for axes which should be updated/deleted
    auto func_axes = func_item->series()->attachedAxes();

    for (auto it = func_axes.begin(); it!=func_axes.end();++it)
    {
        auto axis = *it;
        func_item->series()->detachAxis(axis);

        auto unit_axis = UnitAxis::from_qabstractaxis(axis);

        if (axis->orientation() == Qt::Vertical)
        {
            unit_axis->remove_unit(func_item->value()->codomain()->unit());
            if (unit_axis->no_units())
            {
                chart_->removeAxis(axis);
                delete axis;
                // -------------------------------------------------------------
                // count left and right axes
                // move from one side to another to maintain balance
                // Also we prefer axes on left side
                QAbstractAxis* left = nullptr;
                QAbstractAxis* right = nullptr;
                int left_count = 0;
                int right_count = 0;
                for (auto axis : chart_->axes(Qt::Vertical))
                {
                    if (axis->alignment() == Qt::AlignLeft)
                    {
                        left_count++;
                        left = axis;
                    }
                    else
                    {
                        right_count++;
                        right = axis;
                    }
                }

                if (right_count > left_count)
                    change_axis_alignment(right,Qt::AlignLeft);
                else if (left_count - right_count >= 2)
                    change_axis_alignment(left,Qt::AlignRight);
                // -------------------------------------------------------------
            }
        }
    }

    // removeSeries method detaches axes from series!
    chart_->removeSeries(func_item->series());

    // removing Function from Plot (Plot owns the Function, not FunctionItem!)
    this->value()->remove_function(func_item->value());
    chart_->setTitle(this->value()->name().c_str());

    TreeItem::remove(item);
}

void PlotItem::change_axis_type(QAbstractAxis* axis)
{
    auto alignment = axis->alignment();
    QAbstractAxis* new_axis = nullptr;

    if (auto unit_axis = dynamic_cast<UnitValueAxis*>(axis))
        new_axis = new UnitLogValueAxis(*unit_axis);
    else if (auto unit_axis = dynamic_cast<UnitLogValueAxis*>(axis))
        new_axis = new UnitValueAxis(*unit_axis);

    auto attached_series = axis_series(axis);
    chart_->removeAxis(axis);
    delete axis;

    chart_->addAxis(new_axis,alignment);
    for (auto series : attached_series)
        series->attachAxis(new_axis);
}

