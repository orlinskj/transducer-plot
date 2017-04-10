#ifndef TREEITEM_H
#define TREEITEM_H

#include <memory>
#include <string>

#include "treeitem.h"

// namespace treemodel {

template<typename T> class TreeItemT : public TreeItem
{
public:
    TreeItemT(T* value = nullptr, TreeItem* parent = nullptr);

    virtual T* operator()();
    virtual const T* operator()() const;

    virtual T* value();
    virtual const T* value() const;

    std::string to_string() const override;

protected:
    T* value_;
};

template<typename T> class TreeItemTOwner : public TreeItem
{
public:
    TreeItemTOwner(T* value = nullptr, TreeItem* parent = nullptr);

    virtual T* operator()();
    virtual const T* operator()() const;

    virtual T* value();
    virtual const T* value() const;

    std::string to_string() const override;

protected:
    std::unique_ptr<T> value_;
};

#include "treeitemt.inl"

// }

#endif // TREENODEITEM_H
