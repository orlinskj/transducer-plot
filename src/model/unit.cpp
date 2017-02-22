#include "unit.h"

#include <algorithm>
#include <sstream>
#include <boost/assert.hpp>

Unit::Unit(const std::string& name, const std::string& symbol, const std::string& unit) :
    name_(name), symbol_(symbol), unit_(unit)
{ }

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

std::string Unit::longname() const
{
    return name_ + " [" + unit_ + "] ";
}

Unit Unit::from_symbol(const std::string& s)
{
    auto it = std::find_if(defaults_.begin(), defaults_.end(),
              [s](const Unit& u){ return s == u.symbol(); });

    if (it != defaults_.end())
        return *it;

    return Unit::None;
}

bool operator ==(const Unit& a, const Unit& b)
{
    return a.name() == b.name() && a.symbol() == b.symbol() && a.unit() == b.unit();
}

Unit Unit::None = Unit("","","");

std::vector<Unit> Unit::defaults_ = {
    Unit("częstotliwość", "f", "Hz"),
    Unit("moduł impedancji", "|Z|", "Ω"),
    Unit("kąt fazowy", "theta", "rad"),
    Unit("część rzeczywista", "Re", "Ω"),
    Unit("część urojona", "Im", "Ω")
};

// ---------------
// CombinedUnit
// ---------------

CombinedUnit::CombinedUnit() : Unit("","","")
{ }

CombinedUnit::CombinedUnit(const Unit &u)
{
    add_unit(u);
}

void CombinedUnit::add_unit(const Unit &u)
{
    if (!units_.empty())
    {
        std::stringstream ss;
        ss << "Units " << units_.front().name() << " and "
           << u.name() << " don't match";
        std::string msg = ss.str();
        BOOST_ASSERT_MSG(units_.front().unit() == u.unit(), msg.c_str());
    }

    units_.push_back(u);
    update();
}

void CombinedUnit::remove_unit(const Unit &u)
{
    auto it = std::find(units_.begin(),units_.end(),u);
    if (it != units_.end())
        units_.erase(it);
    update();
}

bool CombinedUnit::match_unit(const Unit &u)
{
    if (units_.empty())
        return true;

    return units_.front().unit() == u.unit();
}

void CombinedUnit::update()
{
    if (units_.empty())
    {
        name_ = "";
        symbol_ = "";
        unit_ = "";
        return;
    }

    name_ = units_.at(0).name();
    symbol_ = units_.at(0).symbol();
    unit_ = units_.at(0).unit();

    for(auto it=units_.begin()+1; it!=units_.end(); ++it)
    {
        name_ += ", " + it->name();
        symbol_ += ", " + it->symbol();
    }
}
