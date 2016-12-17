#include "function.h"

using namespace std;
using namespace ac;

Function::Function(const DataSet &ds, const Series* domain, const Series* codomain, Plot* plot) :
    dataset(ds),
    domain(domain),
    codomain(codomain)
{

}

const Series& Function::getDomain()
{
    return *domain;
}

const Series& Function::getCodomain()
{
    return *codomain;
}

/*void Function::set()
{
    QCustomPlot* plot = this->plot->getPlot();
    graph = plot->addGraph();

    QVector<Series::value_type> domain_values;
    QVector<Series::value_type> codomain_values;

    auto range_x = minmax_element(getDomain().getValues().begin(),
                                  getDomain().getValues().end());
    auto range_y = minmax_element(getCodomain().getValues().begin(),
                                  getCodomain().getValues().end());

    domain_values = QVector<Series::value_type>::fromStdVector(getDomain().getValues());
    codomain_values = QVector<Series::value_type>::fromStdVector(getCodomain().getValues());

    graph->setData(domain_values, codomain_values);

    plot->xAxis->setLabel(getDomain().getLabel().c_str());
    plot->yAxis->setLabel(getCodomain().getLabel().c_str());

    plot->xAxis->setRange(*(range_x.first), *(range_x.second));
    plot->yAxis->setRange(*(range_y.first), *(range_y.second));

    plot->replot();
}*/
