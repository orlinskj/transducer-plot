#include "unit.h"

#include <algorithm>

ac::Unit::Unit(const std::string& name, const std::string& symbol, const std::string& unit) :
    name_(name), symbol_(symbol), unit_(unit)
{
    longname_ = name+std::string(" [")+unit+std::string("]");
}

const std::string& ac::Unit::name() const
{
    return name_;
}

const std::string& ac::Unit::symbol() const
{
    return symbol_;
}

const std::string& ac::Unit::unit() const
{
    return unit_;
}

const std::string& ac::Unit::longname() const
{
    return longname_;
}

ac::Unit ac::Unit::unit_by_symbol(const std::string& symbol)
{
    auto it = std::find_if(defaults_.begin(), defaults_.end(),
              [symbol](const ac::Unit& u){ return symbol == u.symbol(); });

    if (it != defaults_.end())
    {
        return *it;
    }
    else{
        return ac::Unit::None;
    }

}

bool ac::Unit::operator ==(const Unit& u)
{
    return name_ == u.name() && symbol_ == u.symbol() && unit_ == u.unit();
}

ac::Unit ac::Unit::None = Unit("","","");

std::vector<ac::Unit> ac::Unit::defaults_ = {
    Unit("częstotliwość", "f", "Hz"),
    Unit("moduł impedancji", "|Z|", "Ω"),
    Unit("kąt fazowy", "theta", "rad"),
    Unit("część rzeczywista", "Re", "Ω"),
    Unit("część urojona", "Im", "Ω")
};
