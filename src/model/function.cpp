#include "function.h"

using namespace std;
using namespace ac;

Function::Function() : transducer_(nullptr), domain_(nullptr), codomain_(nullptr)
{

}

Function::Function(Transducer* transducer, Set* domain, Set* codomain) :
    transducer_(transducer),
    domain_(domain),
    codomain_(codomain)
{

}

bool Function::operator ==(const Function &f) const
{
    return domain_==f.domain_ && codomain_==f.codomain_ && transducer_==f.transducer_;
}

const Set* Function::get_domain()
{
    return domain_;
}

const Set* Function::get_codomain()
{
    return codomain_;
}

const Transducer* Function::get_transducer()
{
    return transducer_;
}
