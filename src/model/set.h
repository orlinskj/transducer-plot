#ifndef SET_H
#define SET_H

#include <vector>
#include <string>
#include <tuple>

#include "unit.h"


template<typename T> class Set_t;
using SetType = double;
using Set = Set_t<SetType>;

template <typename T>
class Set_t {
public:
    using value_type = T;

    Set_t(std::vector<T>&& values, Unit unit);

    const std::vector<T>& values() const;
    const Unit& unit() const;
    std::tuple<T,T> range() const;

    operator std::string() const;

private:
    Unit unit_;
    std::vector<T> values_;

    // min and max values: { value, index }
    std::tuple<T,size_t> min_;
    std::tuple<T,size_t> max_;
};


#include "set.inl"

#endif // Set_H
