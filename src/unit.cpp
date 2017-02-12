#include "unit.h"

#include <algorithm>

Unit::Unit(const std::string& name, const std::string& symbol, const std::string& unit) :
    name_(name), symbol_(symbol), unit_(unit)
{
    longname_ = name+std::string(" [")+unit+std::string("]");
}

const std::string& Unit::name() const
{
    return name_;
}

const std::string& Unit::symbol() const
{
    return symbol_;
}

const std::string& Unit::unit() const
{
    return unit_;
}

const std::string& Unit::longname() const
{
    return longname_;
}

Unit Unit::unit_by_symbol(const std::string& symbol)
{
    auto it = std::find_if(defaults_.begin(), defaults_.end(),
              [symbol](const Unit& u){ return symbol == u.symbol(); });

    if (it != defaults_.end())
    {
        return *it;
    }
    else{
        return Unit::None;
    }

}

bool Unit::operator ==(const Unit& u)
{
    return name_ == u.name() && symbol_ == u.symbol() && unit_ == u.unit();
}

Unit Unit::None = Unit("","","");

std::vector<Unit> Unit::defaults_ = {
    Unit("częstotliwość", "f", "Hz"),
    Unit("moduł impedancji", "|Z|", "Ω"),
    Unit("kąt fazowy", "theta", "rad"),
    Unit("część rzeczywista", "Re", "Ω"),
    Unit("część urojona", "Im", "Ω")
};
