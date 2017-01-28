#ifndef SET_H
#define SET_H

#include <vector>
#include <string>
#include <tuple>

#include "../unit.h"

namespace ac{


template<typename T> class Set_t;
using Set = Set_t<double>;

template <typename T>
class Set_t {
public:

    using value_type = T;

    Set_t(std::vector<T>&& values, Unit unit);

    const std::vector<T>& values() const;
    const Unit& unit() const;

private:
    Unit unit_;
    std::vector<T> values_;

    // min and max values: { value, index }
    std::tuple<T,size_t> min_;
    std::tuple<T,size_t> max_;
};


}   // namespace ac

#include "set.inl"

#endif // Set_H
