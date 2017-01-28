#ifndef FUNCTION_H
#define FUNCTION_H

#include <string>

#include "treemodel/treeitem.h"
#include "transducer.h"
#include "set.h"

namespace ac{

class Plot;

class Function : public TreeItem {

public:

    Function();
    Function(Transducer* transducer, const Set *domain, const Set *codomain, Plot* plot = nullptr);

    std::string to_string() const override;
    const Set* domain() const;
    const Set* codomain() const;
    const Transducer *transducer() const;

    const Plot* plot() const;

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
