#include "plotadapter.h"
#include <QValueAxis>
#include <QGraphicsLayout>
#include <map>

PlotAdapter::PlotAdapter(Plot *plot) : chart_(new QChart), plot_(plot)
{
    chart_->setMaximumSize(QSize(10000,10000));
    chart_->layout()->setContentsMargins(0,0,0,0);
    chart_->setMargins(QMargins(2,5,2,2));
    chart_->setTitle(plot->description().c_str());
    update();
}

Plot* PlotAdapter::plot() const
{
    return plot_;
}

QChart* PlotAdapter::chart() const
{
    return chart_;
}

void PlotAdapter::update()
{
    chart_->removeAllSeries();
    for (auto& axis : chart_->axes())
        chart_->removeAxis(axis);

    Qt::AlignmentFlag positions[] = { Qt::AlignLeft, Qt::AlignRight };

    // map for storing axes (prevent duplicates)
    std::map<std::string,QAbstractAxis*> codomain_axes;
    std::map<std::string,std::tuple<double,double>> codomain_axes_values;
    QValueAxis* x_axis = nullptr;

    for(auto& item : plot_->children())
    {
        auto func = dynamic_cast<Function*>(item);
        auto codomain_key = func->codomain()->unit().unit();
        auto domain_key = func->domain()->unit().symbol();
        auto it = codomain_axes.find(codomain_key);

        QAbstractAxis* y_axis = nullptr;
        if (it != codomain_axes.end())
        {
            y_axis = (*it).second;
            // adding codoamin name to axis description
            std::string title = func->codomain()->unit().name() + ", " + y_axis->titleText().toStdString();
            y_axis->setTitleText(QString::fromStdString(title));
        }
        else
        {
            y_axis = new QValueAxis;
            y_axis->setTitleText(func->codomain()->unit().longname().c_str());
            dynamic_cast<QValueAxis*>(y_axis)->setLabelFormat("%g");
            chart_->addAxis(y_axis, positions[codomain_axes.size()%2]);
            codomain_axes.emplace(codomain_key,y_axis);
        }

        if (!x_axis)
        {
            x_axis = new QValueAxis;
            x_axis->setTitleText(func->domain()->unit().longname().c_str());
            x_axis->setLabelFormat("%g");
            chart_->addAxis(x_axis,Qt::AlignBottom);
        }

        codomain_axes_values.emplace(codomain_key, func->codomain()->range());

        QLineSeries* series = this->series(func);
        chart_->addSeries(series);

        series->attachAxis(x_axis);
        series->attachAxis(y_axis);
    }
}

QLineSeries* PlotAdapter::series(const Function* func) const
{
    QLineSeries* series = new QLineSeries;
    series->setName(func->codomain()->unit().name().c_str());
    auto domain_values = func->domain()->values();
    auto codomain_values = func->codomain()->values();

    int point_count = std::min(domain_values.size(), codomain_values.size());
    for(int i=0; i<point_count; i++)
        series->append(domain_values.at(i),codomain_values.at(i));

    return series;
}
