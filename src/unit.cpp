#include "unit.h"

#include <algorithm>

ac::Unit::Unit(const std::string& name, const std::string& symbol, const std::string& unit) :
    name_(name), symbol_(symbol), unit_(unit)
{

}

const std::string& ac::Unit::get_name() const
{
    return name_;
}

const std::string& ac::Unit::get_symbol() const
{
    return symbol_;
}

const std::string& ac::Unit::get_unit() const
{
    return unit_;
}

ac::Unit ac::Unit::get_unit_by_symbol(const std::string& symbol)
{
    auto it = std::find_if(defaults_.begin(), defaults_.end(),
              [symbol](const ac::Unit& u){ return symbol == u.get_symbol(); });

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
    return name_ == u.get_name() && symbol_ == u.get_symbol() && unit_ == u.get_unit();
}

ac::Unit ac::Unit::None = Unit("","","");

std::vector<ac::Unit> ac::Unit::defaults_ = {
    Unit("częstotliwość", "f", "Hz"),
    Unit("moduł impedancji", "|Z|", "Ω"),
    Unit("kąt fazowy", "θ", "rad"),
    Unit("część rzeczywista", "Re", "Ω"),
    Unit("część urojona", "Im", "Ω")
};
