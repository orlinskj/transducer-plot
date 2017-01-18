template<typename T>
TreeItem<T>::TreeItem(T* value, TreeNodeItem* parent) :
    TreeNodeItem(parent), value_(value) { }

template<typename T>
T* TreeItem<T>::value() const
{
    return value_.get();
}

template<typename T>
void TreeItem<T>::value(T *value)
{
    value_.reset(value);
}

template<typename T>
std::string TreeItem<T>::to_string() const
{
    return std::string(*value_);
}
