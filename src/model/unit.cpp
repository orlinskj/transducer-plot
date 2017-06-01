#include "unit.h"

#include <algorithm>
#include <sstream>
#include <iostream>
#include <cassert>
#include "../error.h"

const Unit& Unit::record(const std::string& name, const std::string& symbol, const std::string& unit)
{
    Unit u = Unit(name, symbol, unit);
    auto result = Unit::register_.insert(std::make_pair(symbol,u));
    if(!result.second){
        auto existing = Unit::register_.find(symbol);
        std::stringstream ss;
        ss << "Unit with symbol " << symbol << " already registered as '" << (existing->second).name() << "'";
        throw Error(ss.str());
    }
    return (*(result.first)).second;
}

Unit::Unit(const std::string& name, const std::string& symbol, const std::string& unit) :
    name_(name), symbol_(symbol), unit_(unit)
{
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

std::string Unit::longname() const
{
    return name_ + " [" + unit_ + "] ";
}

Unit Unit::from_symbol(const std::string& s)
{
    try{
        return Unit::register_.at(s);
    }
    catch(const std::out_of_range&){
        std::cout << "Unit with symbol " << s << " not found in register.";
        return Unit("",s,"");
    }
}

bool Unit::match_unit(const Unit& u) const
{
    return unit_ == u.unit_;
}

bool operator ==(const Unit& a, const Unit& b)
{
    return a.name() == b.name() && a.symbol() == b.symbol() && a.unit() == b.unit();
}

bool operator !=(const Unit& a, const Unit& b)
{
    return a.name() != b.name() || a.symbol() != b.symbol() || a.unit() != b.unit();
}

std::map<std::string, Unit> Unit::register_;

Unit Unit::None     = Unit::record("","","");
Unit Unit::Voltage  = Unit::record("napięcie", "U", "V");
Unit Unit::Current  = Unit::record("prąd", "I", "A");
Unit Unit::Frequency    = Unit::record("częstotliwość", "f", "Hz");
Unit Unit::PhaseAngle   = Unit::record("kąt fazowy", "theta", "rad");
Unit Unit::Resistance   = Unit::record("rezystancja", "R", "Ω");
Unit Unit::Capacity     = Unit::record("pojemność", "C", "F");
Unit Unit::Inductance   = Unit::record("indukcyjność", "L", "H");
Unit Unit::Impedance    = Unit::record("moduł impedancji", "|Z|", "Ω");
Unit Unit::ImpedanceReal    = Unit::record("część rzeczywista", "Re", "Ω");
Unit Unit::ImpedanceImag    = Unit::record("część urojona", "Im", "Ω");
Unit Unit::Admitance    = Unit::record("moduł admitancji", "|Y|", "S");

std::vector<std::tuple<std::string, double>> Unit::prefixes_ = {
    std::make_tuple(    "E",   1e18 ),
    std::make_tuple(    "P",   1e15 ),
    std::make_tuple(    "T",   1e12 ),
    std::make_tuple(    "G",   1e9  ),
    std::make_tuple(    "M",   1e6  ),
    std::make_tuple(    "k",   1e3  ),
    //std::make_tuple(    "h",   1e2  ),
    //std::make_tuple(    "da",  1e1  ),
    //std::make_tuple(    "c",   1e-1 ),
    std::make_tuple(    "m",   1e-3 ),
    std::make_tuple(    "µ",   1e-6 ),
    std::make_tuple(    "n",   1e-9 ),
    std::make_tuple(    "p",   1e-12),
    std::make_tuple(    "f",   1e-15),
    std::make_tuple(    "a",   1e-18)
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
        assert(units_.front().unit() == u.unit());
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

bool CombinedUnit::match_unit(const CombinedUnit &u) const
{
    if (units_.empty() || u.units_.empty())
        return false;

    return units_.front().match_unit(u.units_.front());
}

bool CombinedUnit::match_unit(const Unit &u) const
{
    if (units_.empty())
        return true;

    return units_.front().match_unit(u);
}

bool CombinedUnit::no_units() const
{
    return units_.empty();
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
