#ifndef SET_H
#define SET_H

#include <vector>
#include <string>
#include <tuple>

#include "unit.h"

namespace ac{

template<typename T> class Set_t;
typedef Set_t<double> Set;

template <typename T>
class Set_t {
public:

    using value_type = T;

    Set_t(vector<T>&& values, Unit unit);

    const vector<T>& get_values() const;
    const Unit& get_unit() const;

private:
    Unit m_unit;
    vector<T> m_values;

    // min and max values: { value, index }
    std::tuple<T,size_t> m_min;
    std::tuple<T,size_t> m_max;
};

}   // namespace ac

#include "set.inl"

#endif // Set_H
