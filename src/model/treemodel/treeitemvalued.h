#ifndef TREEITEM_H
#define TREEITEM_H

#include <memory>
#include <string>

#include "treeitem.h"

template<typename T> class TreeItemValued : public TreeNodeItem
{
public:
    TreeItemValued(T* value = nullptr, TreeNodeItem* parent = nullptr);

    T* value() const;
    void value(T* value);
    std::string to_string() const override;

protected:
    std::unique_ptr<T> value_;
};

#include "treeitemvalued.inl"

#endif // TREENODEITEM_H
