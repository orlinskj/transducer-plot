// using namespace ac;

template <typename T>
Set_t<T>::Set_t(std::vector<T>&& values, Unit unit) :
    unit_(unit), values_(values)
{

}

template <typename T>
const Unit& Set_t<T>::unit() const
{
    return unit_;
}

template <typename T>
const std::vector<T>& Set_t<T>::values() const
{
    return values_;
}

template <typename T>
std::tuple<double,double> Set_t<T>::range() const
{
    return std::make_tuple(std::get<0>(min_),std::get<0>(max_));
}
