#include "plot.h"

using namespace std;
using namespace ac;

void Plot::addFunction(const Function& f)
{
   functions.push_back( unique_ptr<Function>(new Function(f)) );
}

void Plot::addDataSet(DataSet&& dataset)
{
    auto series = dataset.getSeries();
    bool added = false;

    // We're looking for function with domain and codomain labels same
    // as in elements of series.
    for (auto it=functions.begin(); it!=functions.end(); it++)
    {
        const Series* domain = nullptr;
        const Series* codomain = nullptr;

        auto ft = find_if(series.begin(),series.end(),
                            [&domain, &codomain, &it](const Series& s){
            if ((*it)->getDomain().getLabel() == s.getLabel())
                domain = &s;
            if ((*it)->getCodomain().getLabel() == s.getLabel())
                codomain = &s;

            return (domain && codomain);
        });

        if (ft!=series.end())
        {
            added = true;
            this->addFunction(Function(dataset, domain, codomain));
            break;
        }
    }

    if (!added && series.size()>1)
    {
        this->addFunction(Function(dataset, &(series.at(0)), &(series.at(1))));
    }

    sets.push_back(std::move(dataset));
}
