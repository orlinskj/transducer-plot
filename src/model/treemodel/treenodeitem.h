#ifndef TREENODEITEM_H
#define TREENODEITEM_H


#include <vector>
#include <string>
#include <memory>

class TreeNodeItem;

using TreeNodeItemPtr = std::unique_ptr<TreeNodeItem>;

class TreeNodeItem
{
public:
    TreeNodeItem(TreeNodeItem* parent = nullptr);
    virtual ~TreeNodeItem();

    int children_count() const;
    int child_index(TreeNodeItem* child) const;
    TreeNodeItem* child(int index) const;
    TreeNodeItem* parent() const;
    virtual void remove(TreeNodeItem* item);
    virtual TreeNodeItem* append(TreeNodeItem* item);

    virtual std::string to_string() const;

protected:
    TreeNodeItem* parent_;
    std::vector<TreeNodeItemPtr> children_;
};

#endif // TREENODEITEM_H
