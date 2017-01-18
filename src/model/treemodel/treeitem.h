#ifndef TREEITEM_H
#define TREEITEM_H

#include <memory>
#include <string>

#include "treenodeitem.h"

template<typename T> class TreeItem : public TreeNodeItem
{
public:
    TreeItem(T* value = nullptr, TreeNodeItem* parent = nullptr);

    T* value() const;
    void value(T* value);
    std::string to_string() const override;

protected:
    std::unique_ptr<T> value_;
};

#include "treeitem.inl"

#endif // TREENODEITEM_H
