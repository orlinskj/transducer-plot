#include "function.h"

#include <sstream>
#include <functional>
#include <cmath>

using namespace std;

Function::Function(const Function& f) : Function(f.transducer_,f.domain_,f.codomain_)
{ }

Function::Function(const Transducer* transducer, const Set* domain, const Set* codomain) :
    transducer_(transducer),
    domain_(domain),
    codomain_(codomain),
    plot_(nullptr)
{ }

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

const Plot* Function::plot() const
{
    return plot_;
}

std::vector<SetType> Function::values_at(SetType v) const
{
    auto dv = domain()->values();
    auto cv = codomain()->values();

    auto dit = dv.cbegin();
    auto cit = cv.cbegin();

    std::vector<SetType> res;

    while (dit != dv.cend() - 1){
        // whether domain is crossing value @v
        auto dif_left = v - *dit;
        auto dif_right = v - *(dit+1);
        if (dif_left * dif_right <= 0){
            auto da = *dit;
            auto db = *(dit+1);

            auto ca = *cit;
            auto cb = *(cit+1);

            auto val = ca + (cb-ca) * std::abs(dif_left/(db-da));
            res.push_back(val);
        }

        ++dit;
        ++cit;
    }

    std::sort(res.begin(),res.end(),std::greater<SetType>());

    return res;
}

std::string Function::full_name() const
{
    std::stringstream ss;
    if (transducer_)
        ss << "" << transducer_->get_name() << ":    ";
    if (domain_ && codomain_)
        ss << domain_->unit().symbol() << "→" << codomain_->unit().symbol();

    return ss.str();
}

bool operator ==(const Function &f, const Function &g)
{
    return f.domain_==g.domain_ && f.codomain_==g.codomain_;
}

Function::operator std::string() const
{
    return full_name();
}
