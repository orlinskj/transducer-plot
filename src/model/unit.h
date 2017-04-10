#ifndef UNIT_H
#define UNIT_H

#include <string>
#include <vector>
#include <sstream>

class Unit {
public:
    Unit() = default;
    Unit(const Unit& u) = default;
    Unit(const std::string& name, const std::string& symbol, const std::string& unit);

    virtual const std::string& name() const;
    virtual const std::string& symbol() const;
    virtual const std::string& unit() const;
    virtual std::string longname() const;
    template<typename T> std::string nice_repr(T) const;

    static Unit from_symbol(const std::string& s);
    static Unit None;

protected:
    std::string name_;
    std::string symbol_;
    std::string unit_;

    static std::vector<Unit> defaults_;
};

bool operator ==(const Unit& a, const Unit& b);

template <typename T> std::string Unit::nice_repr(T value) const
{
    std::string prefixes = "TGMk mµnp";

    size_t i = prefixes.size() / 2;

    while(value < 0 && i+1<prefixes.size())
    {
        value *= 1e3;
        i++;
    }

    while(value > 1e3 && i>1)
    {
        value /= 1e3;
        i--;
    }

    std::stringstream ss;
    ss << value << prefixes.at(i) << this->unit();
    return ss.str();
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
