#ifndef FUNCTION_H
#define FUNCTION_H

#include <vector>
#include <string>
#include <algorithm>

#include "dataset.h"
#include "series.h"

namespace ac
{

using namespace std;

class Function
{
protected:
    const DataSet& dataset;
    const Series* domain;
    const Series* codomain;

public:
    Function(const DataSet& ds, const Series* domain, const Series* codomain);
    Function(Function&& f);
    Function(const Function& f);
    virtual ~Function();

    void setDomain(const Series* domain);
    void setCodomain(const Series* codomain);

    const Series& getDomain();
    const Series& getCodomain();

    virtual void set();
    virtual void unset();
};

}


#endif // FUNCTION_H
