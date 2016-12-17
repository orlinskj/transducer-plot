#ifndef SERIES_H
#define SERIES_H

#include <vector>
#include <string>

namespace ac{

using namespace std;

template<typename T> class Series_t;
typedef Series_t<double> Series;

template <typename T>
class Series_t {
public:

    using value_type = T;

    Series_t(vector<T>&& values, const string& label);

    const vector<T>& getValues() const;
    const string& getLabel() const;

private:
    vector<T> values;
    string label;
};

}   // namespace ac

#include "series.inl"   // template definitions

#endif // SERIES_H
