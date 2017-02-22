#ifndef UNIT_H
#define UNIT_H

#include <string>
#include <vector>

class Unit {
public:
    Unit() = default;
    Unit(const Unit& u) = default;
    Unit(const std::string& name, const std::string& symbol, const std::string& unit);

    virtual const std::string& name() const;
    virtual const std::string& symbol() const;
    virtual const std::string& unit() const;
    virtual std::string longname() const;

    static Unit from_symbol(const std::string& s);
    static Unit None;

protected:
    std::string name_;
    std::string symbol_;
    std::string unit_;

    static std::vector<Unit> defaults_;
};

bool operator ==(const Unit& a, const Unit& b);


// Class used for stacking few units on one axis.
class CombinedUnit : public Unit {
public:
    CombinedUnit();
    CombinedUnit(const Unit& u);

    void add_unit(const Unit& u);
    void remove_unit(const Unit& u);
private:
    void update();
    std::vector<Unit> units_;
};

#endif // UNIT_H
