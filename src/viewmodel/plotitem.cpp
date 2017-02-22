#include "plotitem.h"

#include <QValueAxis>
#include <QGraphicsLayout>

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

QChart* PlotItem::chart() const
{
    return chart_;
}

/*void PlotItem::update_graphics()
{
    chart()->removeAllSeries();
    for (auto& axis : chart()->axes())
        chart()->removeAxis(axis);

    Qt::AlignmentFlag positions[] = { Qt::AlignLeft, Qt::AlignRight };

    // map for storing axes (prevent duplicates)
    std::map<std::string,QAbstractAxis*> codomain_axes;
    std::map<std::string,std::tuple<double,double>> codomain_axes_values;
    QValueAxis* x_axis = nullptr;

    for(auto& item : children())
    {
        auto func = dynamic_cast<Function*>(item);
        auto codomain_key = func->codomain()->unit().unit();
        auto domain_key = func->domain()->unit().symbol();

        auto it = codomain_axes.find(codomain_key);

        QAbstractAxis* y_axis = nullptr;
        if (it != codomain_axes.end())
        {
            y_axis = (*it).second;
            // adding codomain name to axis description
            std::string title = func->codomain()->unit().name() + ", " + y_axis->titleText().toStdString();
            y_axis->setTitleText(QString::fromStdString(title));
        }
        else
        {
            y_axis = new QValueAxis;
            y_axis->setTitleText(func->codomain()->unit().longname().c_str());
            dynamic_cast<QValueAxis*>(y_axis)->setLabelFormat("%g");
            chart()->addAxis(y_axis, positions[codomain_axes.size()%2]);
            codomain_axes.emplace(codomain_key,y_axis);
        }

        if (!x_axis)
        {
            x_axis = new QValueAxis;
            x_axis->setTitleText(func->domain()->unit().longname().c_str());
            x_axis->setLabelFormat("%g");
            chart()->addAxis(x_axis,Qt::AlignBottom);
        }

        codomain_axes_values.emplace(codomain_key, func->codomain()->range());

        //QLineSeries* series = this->create_series(func);
        //chart()->addSeries(series);

        //series->attachAxis(x_axis);
        //series->attachAxis(y_axis);
    }
}*/

TreeItem* PlotItem::append(TreeItem* item)
{
    auto func_item = dynamic_cast<FunctionItem*>(item);

    chart_->addSeries(func_item->series());

    // looking for codomain axes in plot, if absent - create
    auto axes = chart_->axes(Qt::Vertical);
    auto axis_it = std::find_if(axes.cbegin(), axes.cend(), [func_item](auto axis){
        auto unit_axis = UnitAxis::from_qabstractaxis(axis);
        /*if (unit_axis->unit_match(func_item->value()->codomain()->unit()))
            return true;*/
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
        chart_->addAxis(axis,Qt::AlignLeft);
        func_item->series()->attachAxis(axis);
    }

    auto x_axis = chart_->axisX();
    if (!x_axis)
    {
        auto domain_axis = new UnitValueAxis(func_item->value()->domain()->unit());
        chart_->addAxis(domain_axis,Qt::AlignBottom);
        func_item->series()->attachAxis(domain_axis);
    }

    return TreeItem::append(item);
}

