#ifndef FUNCTION_H
#define FUNCTION_H

#include "transducer.h"
#include "set.h"
#include "plot.h"

namespace ac
{

class Function {

public:
    Function(const DataSet& dataset, const Set* domain, const Set* codomain);

    const Set& get_domain();
    const Set& get_codomain();

protected:
    const DataSet& _dataset;
    const Set* _domain;
    const Set* _codomain;
};

}


#endif // FUNCTION_H
