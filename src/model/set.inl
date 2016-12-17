using namespace std;
using namespace ac;

template <typename T>
Series_t<T>::Series_t(vector<T>&& values, const string& label) :
    values(values), label(label)
{

}

template <typename T>
const string& Series_t<T>::getLabel() const
{
    return label;
}

template <typename T>
const vector<T>& Series_t<T>::getValues() const
{
    return values;
}
