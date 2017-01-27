template<typename T>
TreeItemValued<T>::TreeItemValued(T* value, TreeItem* parent) :
    TreeItem(parent), value_(value) { }

template<typename T>
T* TreeItemValued<T>::value() const
{
    return value_.get();
}

template<typename T>
void TreeItemValued<T>::value(T *value)
{
    value_.reset(value);
}

template<typename T>
std::string TreeItemValued<T>::to_string() const
{
    return std::string(*value_);
}
