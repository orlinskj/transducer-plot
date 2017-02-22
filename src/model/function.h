#ifndef FUNCTION_H
#define FUNCTION_H

#include <string>
#include <boost/optional.hpp>

#include "transducer.h"
#include "set.h"


class Plot;

class Function {

public:
    Function() = default;
    Function(const Function& f);
    Function(const Transducer* transducer, const Set *domain, const Set *codomain);

    std::string full_name() const;
    const Set* domain() const;
    const Set* codomain() const;
    const Transducer *transducer() const;

    const Plot* plot() const;
    boost::optional<SetType> value_at(SetType v) const;

    operator std::string() const;

protected:
    const Transducer* transducer_;
    const Set* domain_;
    const Set* codomain_;

    const Plot* plot_; // plot is an owner of function

    friend class Plot;
    friend bool operator==(const Function& f, const Function& g);
};


#endif // FUNCTION_H
