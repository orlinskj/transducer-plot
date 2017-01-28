#include "function.h"

#include <sstream>

using namespace std;
using namespace ac;

Function::Function() :
    transducer_(nullptr),
    domain_(nullptr),
    codomain_(nullptr),
    plot_(nullptr)
{

}

Function::Function(Transducer* transducer, const Set* domain, const Set* codomain, Plot *plot) :
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

const Set* Function::domain() const
{
    return domain_;
}

const Set* Function::codomain() const
{
    return codomain_;
}

const Transducer* Function::transducer() const
{
    return transducer_;
}

const ac::Plot* ac::Function::plot() const
{
    return plot_;
}

void ac::Function::set_plot(const ac::Plot* plot)
{
    plot_ = plot;
}

std::string ac::Function::to_string() const
{
    std::stringstream ss;
    if (transducer_)
        ss << "[" << transducer_->get_name() << "] ";
    if (domain_ && codomain_)
        ss << domain_->unit().symbol() << " -> " << codomain_->unit().symbol();

    return ss.str();
}
