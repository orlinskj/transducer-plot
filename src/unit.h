#ifndef UNIT_H
#define UNIT_H

#include <string>
#include <vector>

// namespace ac {

class Unit {
public:
    Unit(const std::string& name, const std::string& symbol, const std::string& unit);

    const std::string& symbol() const;
    const std::string& name() const;
    const std::string& unit() const;
    const std::string& longname() const;

    static Unit unit_by_symbol(const std::string& symbol);

    static Unit None;

    bool operator==(const Unit& u);

private:
    std::string name_;
    std::string symbol_;
    std::string unit_;
    std::string longname_;

    static std::vector<Unit> defaults_;
};

// } // namespace ac

#endif // UNIT_H
