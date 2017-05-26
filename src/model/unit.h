#ifndef UNIT_H
#define UNIT_H

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <map>
#include <tuple>
#include <utf8.h>

class Unit {
public:
    static Unit from_symbol(const std::string& s);
    static const Unit& record(const std::string& name, const std::string& symbol, const std::string& unit);

    static Unit None;
    static Unit Voltage;
    static Unit Current;
    static Unit Frequency;
    static Unit PhaseAngle;
    static Unit Resistance;
    static Unit Capacity;
    static Unit Inductance;
    static Unit Impedance;
    static Unit ImpedanceReal;
    static Unit ImpedanceImag;
    static Unit Admitance;

    Unit() = default;
    Unit(const Unit& u) = default;
    Unit(const std::string& name, const std::string& symbol, const std::string& unit);

    virtual const std::string& name() const;
    virtual const std::string& symbol() const;
    virtual const std::string& unit() const;
    virtual std::string longname() const;

    template<typename T> std::string nice_repr(T,size_t = 6) const;
    template<typename T> T value_from_repr(const std::string& repr) const;

protected:
    std::string name_;
    std::string symbol_;
    std::string unit_;

    static std::map<std::string, Unit> register_;
    static std::vector<std::tuple<std::string,double>> prefixes_;
};

bool operator ==(const Unit& a, const Unit& b);

template <typename T> std::string Unit::nice_repr(T value, size_t prec) const
{
    std::string prefixes = "TGMk mµnp";
    auto it = std::find(prefixes.begin(),prefixes.end(),' ');

    while(std::abs(value) < 1)
    {
        utf8::advance(it,1,prefixes.end());
        if (it != prefixes.end())
            value *= 1e3;
        else
        {
            utf8::prior(it,prefixes.begin());
            break;
        }
    }

    while(std::abs(value) >= 1e3 && it != prefixes.begin())
    {
        utf8::prior(it,prefixes.begin());
        value /= 1e3;
    }

    std::stringstream ss;
    auto start = it;
    utf8::next(it,prefixes.end());
    std::string p(start,it);
    if (p == " ")
        p.clear();
    ss << std::setprecision(prec) << value << " " << p << this->unit();
    return ss.str();
}

template <typename T> T Unit::value_from_repr(const std::string& repr) const
{
    std::string wo_spaces;
    for (auto c : repr)
        if (c != ' ')
            wo_spaces += c;

    double value;
    std::string prefix;
    std::stringstream ss(wo_spaces);

    ss >> value >> prefix;

    size_t i = prefix.find(this->unit());
    if (i != std::string::npos && this->unit().length() + i == prefix.length())
    {
        prefix.erase(i);
    }

    auto it = std::find_if(prefixes_.begin(), prefixes_.end(), [prefix](const auto& p)
    {
        return std::get<0>(p) == prefix;
    });

    if (it != prefixes_.end())
    {
        value *= std::get<1>(*it);
    }

    return value;
}


// Class used for stacking few units on one axis.
class CombinedUnit : public Unit {
public:
    CombinedUnit();
    CombinedUnit(const Unit& u);

    void add_unit(const Unit& u);
    void remove_unit(const Unit& u);
    bool match_unit(const Unit& u) const;
    bool no_units() const;
private:
    void update();
    std::vector<Unit> units_;
};

#endif // UNIT_H
