#include "plotitem.h"

#include <QValueAxis>
#include <QGraphicsLayout>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QDebug>
#include <QGraphicsScene>

#include <map>
#include <regex>
#include <algorithm>

#include "functionitem.h"
#include "../view/unitaxis.h"
#include "../view/minmaxlayer.h"
#include "../viewmodel/treemodel/treeitemmodel.h"
#include "../error.h"

bool PlotItem::isFuncAdmitanceRing(FunctionItem *func)
{
    if (!func)
        return false;

    auto du = func->value()->domain()->unit();
    auto cu = func->value()->codomain()->unit();

    return (du == Unit::ImpedanceReal && cu == Unit::ImpedanceImag) ||
           (du == Unit::ImpedanceImag && cu == Unit::ImpedanceReal);
}

PlotItem::PlotItem() : TreeItemTOwner<Plot>(new Plot), chart_(new QChart)
{
    //chart_->layout()->setContentsMargins(5,5,5,5);
    chart_->setMargins(QMargins(0,0,0,0));
    layers_.add_layer(new MinMaxLayer(this));
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

LayerStack& PlotItem::layers()
{
    return layers_;
}

TreeItem* PlotItem::find_same_as(TreeItem *item)
{
    auto input_func = dynamic_cast<FunctionItem*>(item);
    if (!input_func)
        return nullptr;

    for (auto item : this->children()){
        auto func_item = dynamic_cast<FunctionItem*>(item);
        if (!func_item)
            continue;
        auto func = func_item->value();

        if (func->domain() == input_func->value()->domain() &&
            func->codomain() == input_func->value()->codomain())
            return func_item;
    }

    return nullptr;
}

TreeItem* PlotItem::append(TreeItem* item)
{
    auto func_item = dynamic_cast<FunctionItem*>(item);

    auto existing_func = dynamic_cast<FunctionItem*>(this->child(0));
    if (existing_func && existing_func->value()->domain()->unit().unit() != func_item->value()->domain()->unit().unit()){
        throw Error(QObject::tr("Nie udało się dodać funkcji. Aplikacja nie obsługuje osi X wykresu z wieloma różnymi jednostkami.").toStdString());
    }

    if (find_same_as(item)){
        throw Error(QObject::tr("Wybrana funkcja istnieje już na wykresie").toStdString(), Error::Type::Info);
    }

    Qt::AlignmentFlag positions[] = { Qt::AlignLeft, Qt::AlignRight };

    // transfering ownership of Function object to Plot
    this->value()->add_function(func_item->value());
    chart_->setTitle(this->value()->name().c_str());
    chart_->addSeries(func_item->series());

    // looking for codomain axes in plot, if absent - create
    auto axes = chart_->axes(Qt::Vertical);
    auto axis_it = std::find_if(axes.cbegin(), axes.cend(), [func_item](auto axis){
        auto unit_axis = UnitAxis::from_qabstractaxis(axis);
        if (unit_axis->unit().match_unit(func_item->value()->codomain()->unit())){
            return true;
        }
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
            unit_axis->unit().add_unit(func_item->value()->codomain()->unit());
        else
            throw std::runtime_error("Chart axis is neither UnitValueAxis nor UnitLogValueAxis");
        axis->setTitleText(unit_axis->unit().longname().c_str());

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

    // from here FunctionItem is valid child of PlotItem
    auto ret = TreeItem::append(item);


    // layer
    if (isFuncAdmitanceRing(func_item)){
        layers_.add_layer(new AdmitanceRingLayer(func_item));
    }

    return ret;
}

void PlotItem::resize(QSize size)
{
    chart_->resize(size);
    layers_.resize(size);

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

TreeItem::iterator PlotItem::remove(TreeItem *item)
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
            auto temp_unit = func_item->value()->codomain()->unit();
            unit_axis->unit().remove_unit(func_item->value()->codomain()->unit());
            if (unit_axis->unit().no_units())
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
            else{
                axis->setTitleText(QString::fromStdString(unit_axis->unit().longname()));
            }
        }
    }

    // remove all layers belonging to a function
    layers_.remove_func_layers(func_item);

    // removeSeries method detaches axes from series!
    chart_->removeSeries(func_item->series());

    // removing Function from Plot (Plot owns the Function, not FunctionItem!)
    this->value()->remove_function(func_item->value());
    chart_->setTitle(this->value()->name().c_str());

    // MOST IMPORTANT!
    return TreeItem::remove(item);
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

PlotItem* PlotItem::from_qmodelindex(const QModelIndex &index)
{
    return TreeItem::from_qmodelindex<PlotItem>(index);
    //return dynamic_cast<PlotItem*>(index.data(TreeItem::Role).value<TreeItem*>());
}

