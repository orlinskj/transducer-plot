#ifndef FUNCTION_H
#define FUNCTION_H

#include <string>

#include "plotstoreitem.h"
#include "transducer.h"
#include "set.h"

namespace ac{

class Plot;

class Function : public PlotStoreItem {

public:

    Function();
    Function(Transducer* transducer, Set* domain, Set* codomain, Plot* plot);

    std::string to_string() const override;
    const Set* get_domain() const;
    const Set* get_codomain() const;
    const Transducer *get_transducer() const;

    const Plot* get_plot() const;

    bool operator==(const Function& f) const;

protected:
    const Transducer* transducer_;
    const Set* domain_;
    const Set* codomain_;

    const Plot* plot_;
};


}   // namespace ac

Q_DECLARE_METATYPE(ac::Function)

#endif // FUNCTION_H
