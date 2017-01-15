#include "function.h"

using namespace std;
using namespace ac;

Function::Function() : transducer_(nullptr), domain_(nullptr), codomain_(nullptr)
{

}

Function::Function(Transducer* transducer, Set* domain, Set* codomain, Plot *plot) :
    transducer_(transducer),
    domain_(domain),
    codomain_(codomain),
    plot_(plot)
{

}

bool Function::operator ==(const Function &f) const
{
    return domain_==f.domain_ && codomain_==f.codomain_ && transducer_==f.transducer_;
}

const Set* Function::get_domain() const
{
    return domain_;
}

const Set* Function::get_codomain() const
{
    return codomain_;
}

const Transducer* Function::get_transducer() const
{
    return transducer_;
}

const ac::Plot* ac::Function::get_plot() const
{
    return plot_;
}

std::string ac::Function::to_string() const
{
    return std::string("Funkcja");
}
