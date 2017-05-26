#include "transducer.h"

using namespace std;

Transducer::Transducer(const std::string &name,
                 const std::string &source,
                 Description &&description,
                 std::vector<Set> &&sets) :
    name_(name),
    source_(source),
    description_(description),
    sets_(sets)
{

}

/*Transducer::Transducer(Transducer &&t) :
    name_(t.name_),
    source_(t.source_),
    description_(description_)
{

}*/

const string& Transducer::get_name() const
{
    return name_;
}

const string& Transducer::get_source() const
{
    return source_;
}

const Transducer::Description& Transducer::get_description() const
{
    return description_;
}

const Set* Transducer::get_set(const Unit& unit) const
{
    auto it = std::find_if(sets_.begin(), sets_.end(),
        [unit](const auto& s){
            return s.unit() == unit;
        });

    if (it != sets_.end())
        return &(*it);

    return nullptr;
}

std::vector<Set>& Transducer::get_sets()
{
    return sets_;
}

Transducer::operator const std::string &() const
{
    return name_;
}
