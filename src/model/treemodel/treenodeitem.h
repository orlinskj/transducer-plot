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

    virtual int children_count() const;
    virtual int child_index(const TreeNodeItem *child) const;
    virtual TreeNodeItem* child(int index) const;
    virtual TreeNodeItem* parent() const;
    virtual void remove(TreeNodeItem* item);
    virtual TreeNodeItem* append(TreeNodeItem* item);
    virtual int index() const;
    virtual void kill();

    // these are only for Qt
    virtual void emit_begin_insert_rows(int first, int last, std::vector<int>* tree);
    virtual void emit_end_insert_rows();
    virtual void emit_begin_remove_rows(int first, int last, std::vector<int>* tree);
    virtual void emit_end_remove_rows();

    virtual std::string to_string() const;

protected:
    TreeNodeItem* parent_;
    std::vector<TreeNodeItemPtr> children_;
};

#endif // TREENODEITEM_H
