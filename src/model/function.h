#ifndef FUNCTION_H
#define FUNCTION_H

#include "transducer.h"
#include "set.h"

namespace ac{

class Function {

public:
    static constexpr int Role = Qt::UserRole + 1;

    Function();
    Function(Transducer* transducer, Set* domain, Set* codomain);

    const Set* get_domain();
    const Set* get_codomain();
    const Transducer *get_transducer();

    bool operator==(const Function& f) const;

protected:
    const Transducer* transducer_;
    const Set* domain_;
    const Set* codomain_;
};


}   // namespace ac

Q_DECLARE_METATYPE(ac::Function)

#endif // FUNCTION_H
