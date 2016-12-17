#include "transducer.h"

using namespace ac;
using namespace std;

DataSet::DataSet(const string &name,
                 const string &source,
                 Description &&description,
                 vector<Series> &&series) :
    name(name),
    source(source),
    description(description),
    series(series)
{

}

const string& DataSet::getName() const
{
    return name;
}

const string& DataSet::getSource() const
{
    return source;
}

const DataSet::Description& DataSet::getDescription() const
{
    return description;
}

const vector<Series>& DataSet::getSeries() const
{
    return series;
}
