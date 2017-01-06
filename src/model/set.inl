using namespace ac;

template <typename T>
Set_t<T>::Set_t(std::vector<T>&& values, Unit unit) :
    unit_(unit), values_(values)
{

}

template <typename T>
const Unit& Set_t<T>::get_unit() const
{
    return unit_;
}

template <typename T>
const std::vector<T>& Set_t<T>::get_values() const
{
    return values_;
}
