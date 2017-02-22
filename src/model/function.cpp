#include "function.h"

#include <sstream>

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

boost::optional<SetType> Function::value_at(SetType v) const
{
    auto dv = domain()->values();
    auto cv = codomain()->values();

    auto beg = dv.begin();
    auto end = dv.end();
    auto m = beg;

    SetType eps{1e-8};

    while(true)
    {
        // middle between beg and end
        m = beg + (end - beg - 1)/2;

        if (*m < v-eps)
            beg = m;
        else if (*m > v+eps)
            end = m + 1;
        else
            break;

        if (end - beg <= 2)
        {
            if ((v-*beg) * (v-*(end-1)) > 0.0)
                return boost::none;
            else
            {
                /*if (std::abs(v-*beg) < std::abs(v-*m))
                    return boost::optional<SetType>(cv.at(beg-dv.begin()));
                else
                    return boost::optional<SetType>(cv.at(m-dv.begin()));*/

                // interpolate between anchors (beg) and (end-1)
                auto dv_a = *beg;
                auto dv_b = *(end-1);

                auto cv_a = cv.at(beg - dv.begin());
                auto cv_b = cv.at(end-1 - dv.begin());

                return boost::optional<SetType>(
                            cv_a + (cv_b-cv_a) * (v-dv_a) / (dv_b-dv_a)
                            );
            }
        }
    }

    return boost::optional<SetType>(*m);
}

std::string Function::full_name() const
{
    std::stringstream ss;
    if (transducer_)
        ss << "[" << transducer_->get_name() << "] ";
    if (domain_ && codomain_)
        ss << domain_->unit().symbol() << " -> " << codomain_->unit().symbol();

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
