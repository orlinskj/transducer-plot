#include "treenodeitem.h"

#include <algorithm>

TreeNodeItem::TreeNodeItem(TreeNodeItem *parent) : parent_(parent) { }

TreeNodeItem::~TreeNodeItem() { }

int TreeNodeItem::children_count() const
{
    return int(children_.size());
}

int TreeNodeItem::child_index(const TreeNodeItem *child) const
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

int TreeNodeItem::index() const
{
    if (parent_)
        return parent_->child_index(this);
    else
        return -1;
}

void TreeNodeItem::remove(TreeNodeItem* item)
{
    auto it = std::find_if(children_.begin(), children_.end(),
                           [item](const TreeNodeItemPtr& p){ return item == p.get(); });

    if (it != children_.end())
    {
        int row = child_index(it->get());
        emit_begin_remove_rows(row, row, nullptr);
        children_.erase(it);
        emit_end_remove_rows();
    }
}

TreeNodeItem* TreeNodeItem::append(TreeNodeItem* item)
{
    emit_begin_insert_rows(children_count(),children_count(), nullptr);
    item->parent_ = this;
    children_.emplace_back(TreeNodeItemPtr(item));
    emit_end_insert_rows();
    return item;
}

void TreeNodeItem::kill()
{
    if (parent_)
        parent_->remove(this);
}

std::string TreeNodeItem::to_string() const
{
    return std::string("TreeItem");
}

// ------ only for Qt ---------------------

void TreeNodeItem::emit_begin_insert_rows(int first, int last, std::vector<int>* tree)
{
    if (parent_)
    {
        std::unique_ptr<std::vector<int> > tree_ptr(new std::vector<int>());
        if (!tree)
            tree = tree_ptr.get();

        tree->push_back(this->index());
        parent_->emit_begin_insert_rows(first, last, tree);
    }
}

void TreeNodeItem::emit_end_insert_rows()
{
    if (parent_)
        parent_->emit_end_insert_rows();
}

void TreeNodeItem::emit_begin_remove_rows(int first, int last, std::vector<int>* tree)
{
    if (parent_)
    {
        std::unique_ptr<std::vector<int>> tree_ptr{ new std::vector<int>()};
        if (!tree)
            tree = tree_ptr.get();

        tree->push_back(this->index());
        parent_->emit_begin_remove_rows(first, last, tree);
    }
}

void TreeNodeItem::emit_end_remove_rows()
{
    if (parent_)
        parent_->emit_end_remove_rows();
}
