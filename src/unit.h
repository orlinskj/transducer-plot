#ifndef UNIT_H
#define UNIT_H

#include <string>

namespace ac {

class Unit {
public:
    Unit(const std::string& name, const std::string& symbol);

private:
    std::string m_name;
    std::string m_symbol;
};

}

#endif // UNIT_H
