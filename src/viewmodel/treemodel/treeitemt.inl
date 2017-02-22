template<typename T>
TreeItemT<T>::TreeItemT(T* value, TreeItem* parent) :
    TreeItem(parent), value_(value) { }

template<typename T>
T* TreeItemT<T>::operator()()
{
    return value_;
}

template<typename T>
const T* TreeItemT<T>::operator()() const
{
    return value_;
}

template<typename T>
T* TreeItemT<T>::value()
{
    return value_;
}

template<typename T>
const T* TreeItemT<T>::value() const
{
    return value_;
}

template<typename T>
std::string TreeItemT<T>::to_string() const
{
    return std::string(*value_);
}

// --------------------
// TreeItemTOwner
// --------------------

template<typename T>
TreeItemTOwner<T>::TreeItemTOwner(T* value, TreeItem* parent) :
    TreeItem(parent), value_(value) { }

template<typename T>
T* TreeItemTOwner<T>::operator()()
{
    return value_.get();
}

template<typename T>
const T* TreeItemTOwner<T>::operator()() const
{
    return value_.get();
}

template<typename T>
T* TreeItemTOwner<T>::value()
{
    return value_.get();
}

template<typename T>
const T* TreeItemTOwner<T>::value() const
{
    return value_.get();
}

template<typename T>
std::string TreeItemTOwner<T>::to_string() const
{
    return std::string(*value_);
}

