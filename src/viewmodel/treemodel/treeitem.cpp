#include "treeitem.h"

#include <algorithm>

TreeItem::TreeItem(TreeItem *parent) :
    parent_(parent), ancestor_count_(0) { }

TreeItem::~TreeItem() {
    for (const auto& item : children_)
        delete item;
}

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
    auto it = std::find_if(children_.cbegin(), children_.cend(),
                           [child](const TreeItem* p){ return p == child; });
    if (it != children_.cend())
        return std::distance(children_.cbegin(), it);

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
    if (index >= 0 && index < children_count())
        return children_.at(index);
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

TreeItem::iterator TreeItem::remove(TreeItem* item)
{
    auto it = std::find_if(children_.cbegin(), children_.cend(),
                           [item](const TreeItem* p){ return item == p; });

    iterator next_it = children_.end();

    if (it != children_.cend())
    {
        int row = child_index(*it);
        emit_begin_remove_rows(row,row,this);
        next_it = children_.erase(it);
        for (TreeItem* tit=this; tit!=nullptr; tit=tit->parent())
            tit->ancestor_count_--;
        emit_end_remove_rows(row,row,this);
    }

    return next_it;
}

TreeItem* TreeItem::append(TreeItem* item)
{
    int row = children_count();
    emit_begin_insert_rows(row,row,this);
    item->parent_ = static_cast<TreeItem*>(this);
    children_.push_back(item);
    for(TreeItem* tit=this; tit!=nullptr; tit=tit->parent())
        tit->ancestor_count_++;
    emit_end_insert_rows(row,row,this);
    return item;
}

void TreeItem::path_to_root(std::vector<TreeItem *>* path) const
{
    for(TreeItem* it=parent(); it!=nullptr; it=it->parent())
        path->push_back(it);
}

bool TreeItem::is_root() const
{
    return !parent();
}

TreeItem* TreeItem::root()
{
    if (parent())
        return parent()->root();
    else
        return this;
}

void TreeItem::kill()
{
    if (parent_)
        parent_->remove(this);
}

void TreeItem::kill_children()
{
    if (!children_.empty())
    {
        int last = children_count() - 1;
        emit_begin_remove_rows(0,last,this);
        children_.clear();
        emit_end_remove_rows(0,last,this);
    }
}

std::string TreeItem::to_string() const
{
    return std::string("TreeItem");
}

const std::vector<TreeItem*>& TreeItem::children()
{
    return children_;
}

// ---------------------------- SIGNALS ---------------------

void TreeItem::emit_begin_insert_rows(int first, int last, TreeItem* parent)
{
    if (parent_)
        parent_->emit_begin_insert_rows(first, last, parent);
}

void TreeItem::emit_end_insert_rows(int first, int last, TreeItem* parent)
{
    if (parent_)
        parent_->emit_end_insert_rows(first,last,parent);
}

void TreeItem::emit_begin_remove_rows(int first, int last, TreeItem* parent)
{
    if (parent_)
        parent_->emit_begin_remove_rows(first,last,parent);
}

void TreeItem::emit_end_remove_rows(int first, int last, TreeItem* parent)
{
    if (parent_)
        parent_->emit_end_remove_rows(first,last,parent);
}
