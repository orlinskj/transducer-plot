#include "treenodeitem.h"

#include <algorithm>

TreeNodeItem::TreeNodeItem(TreeNodeItem *parent) : parent_(parent) { }

TreeNodeItem::~TreeNodeItem() { }

int TreeNodeItem::children_count() const
{
    return int(children_.size());
}

int TreeNodeItem::child_index(TreeNodeItem *child) const
{
    auto it = std::find_if(children_.begin(), children_.end(),
                           [child](const TreeNodeItemPtr& p){ return p.get()==child; });
    if (it != children_.end())
        return int(it-children_.begin());

    return -1;
}

TreeNodeItem* TreeNodeItem::child(int index) const
{
    if (index >= 0 && index < int(children_.size()))
        return children_.at(index).get();
    else
        return nullptr;
}

TreeNodeItem* TreeNodeItem::parent() const
{
    return parent_;
}

void TreeNodeItem::remove(TreeNodeItem* item)
{
    auto it = std::find_if(children_.begin(), children_.end(),
                           [item](const TreeNodeItemPtr& p){ return item == p.get(); });
    if (it != children_.end())
        children_.erase(it);
}

TreeNodeItem* TreeNodeItem::append(TreeNodeItem* item)
{
    item->parent_ = this;
    children_.emplace_back(TreeNodeItemPtr(item));

    return item;
}

std::string TreeNodeItem::to_string() const
{
    return std::string("TreeItem");
}
