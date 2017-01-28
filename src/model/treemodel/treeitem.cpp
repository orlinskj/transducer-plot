#include "treeitem.h"

#include <algorithm>

TreeItem::TreeItem(TreeItem *parent) :
    parent_(parent), ancestor_count_(0), children_weak_valid_(false) { }

TreeItem::~TreeItem() { }

int TreeItem::children_count() const
{
    return int(children_.size());
}

int TreeItem::ancestor_count() const
{
    return ancestor_count_;
}

int TreeItem::child_index(const TreeItem *child) const
{
    auto it = std::find_if(children_.begin(), children_.end(),
                           [child](const TreeItemPtr& p){ return p.get()==child; });
    if (it != children_.end())
        return int(it-children_.begin());

    return -1;
}

int TreeItem::index() const
{
    if (parent_)
        return parent_->child_index(this);
    else
        return -1;
}

int TreeItem::absolute_index(TreeItem *relative) const
{
    std::vector<TreeItem*> path;
    path_to_root(&path);

    // if relative descendant is not set, set it to root element
    if (!path.size())
        return 0;

    if (!relative)
        relative = path.back();

    int count = 0;
    for(const TreeItem* it=this; it && it!=relative; it=it->parent())
    {
        for(int i=0; i<it->index() && it->parent(); i++)
        {
            count += it->parent()->child(i)->ancestor_count() + 1;
        }

        count++;
    }

    return count;
}

TreeItem* TreeItem::child(int index) const
{
    if (index >= 0 && index < int(children_.size()))
        return children_.at(index).get();
    else
        return nullptr;
}

TreeItem* TreeItem::parent() const
{
    return parent_;
}

const TreeItem* TreeItem::absolute_child(int index) const
{
    int child_index;

    if (index == 0)
        return this;

    index -= 1;

    for(child_index=0; child_index<children_count(); child_index++)
    {
        if (index < child(child_index)->ancestor_count()+1)
            break;
        else
            index -= child(child_index)->ancestor_count()+1;
    }

    if (child_index >= children_count())
        return nullptr;

    return child(child_index)->absolute_child(index);
}

void TreeItem::remove(TreeItem* item)
{
    auto it = std::find_if(children_.begin(), children_.end(),
                           [item](const TreeItemPtr& p){ return item == p.get(); });

    if (it != children_.end())
    {
        int row = child_index(it->get());
        emit_begin_remove_rows(row, row, nullptr);
        children_.erase(it);
        for (TreeItem* it=this; it!=nullptr; it=it->parent())
            it->ancestor_count_--;
        children_weak_valid_ = false;
        emit_end_remove_rows();
    }
}

TreeItem* TreeItem::append(TreeItem* item)
{
    emit_begin_insert_rows(children_count(),children_count(), nullptr);
    item->parent_ = static_cast<TreeItem*>(this);
    children_.emplace_back(TreeItemPtr(item));
    for(TreeItem* it=this; it!=nullptr; it=it->parent())
        it->ancestor_count_++;
    children_weak_valid_ = false;
    emit_end_insert_rows();
    return item;
}

void TreeItem::path_to_root(std::vector<TreeItem *>* path) const
{
    for(TreeItem* it=parent(); it!=nullptr; it=it->parent())
        path->push_back(it);
}

void TreeItem::kill()
{
    if (parent_)
        parent_->remove(this);
}

void TreeItem::kill_children()
{
    emit_begin_remove_rows(0,children_count()-1,nullptr);
    children_.clear();
    children_weak_valid_ = false;
    emit_end_remove_rows();
}

std::string TreeItem::to_string() const
{
    return std::string("TreeItem");
}

const std::vector<TreeItem*>& TreeItem::children()
{
    if (!children_weak_valid_)
    {
        children_weak_.clear();
        for(auto &ptr : children_)
            children_weak_.push_back(ptr.get());
        children_weak_valid_ = true;
    }

    return children_weak_;
}

// ------ only for Qt ---------------------

void TreeItem::emit_begin_insert_rows(int first, int last, std::vector<int>* tree)
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

void TreeItem::emit_end_insert_rows()
{
    if (parent_)
        parent_->emit_end_insert_rows();
}

void TreeItem::emit_begin_remove_rows(int first, int last, std::vector<int>* tree)
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

void TreeItem::emit_end_remove_rows()
{
    if (parent_)
        parent_->emit_end_remove_rows();
}
