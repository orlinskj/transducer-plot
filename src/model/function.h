#ifndef FUNCTION_H
#define FUNCTION_H

#include <string>

#include "treemodel/treenodeitem.h"
#include "transducer.h"
#include "set.h"

namespace ac{

class Plot;

class Function : public TreeNodeItem {

public:

    Function();
    Function(Transducer* transducer, Set* domain, Set* codomain, Plot* plot = nullptr);

    std::string to_string() const override;
    const Set* get_domain() const;
    const Set* get_codomain() const;
    const Transducer *get_transducer() const;

    const Plot* get_plot() const;

    void set_plot(const Plot* plot);

    bool operator==(const Function& f) const;

protected:
    const Transducer* transducer_;
    const Set* domain_;
    const Set* codomain_;

    const Plot* plot_;
};


}   // namespace ac

Q_DECLARE_METATYPE(ac::Function*)

#endif // FUNCTION_H
