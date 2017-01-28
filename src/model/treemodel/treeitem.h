#ifndef TREENODEITEM_H
#define TREENODEITEM_H


#include <vector>
#include <string>
#include <memory>

class TreeItem;

using TreeItemPtr = std::unique_ptr<TreeItem>;

class TreeItem
{
public:
    TreeItem(TreeItem* parent = nullptr);
    virtual ~TreeItem();

    virtual int children_count() const;
    virtual int ancestor_count() const;

    virtual int child_index(const TreeItem *child) const;
    virtual int index() const;
    virtual int absolute_index(TreeItem* relative=nullptr) const;
    virtual const std::vector<TreeItem*>& children();

    virtual TreeItem* child(int index) const;
    virtual TreeItem* parent() const;
    virtual const TreeItem* absolute_child(int index) const;
    virtual void path_to_root(std::vector<TreeItem*>* path) const;

    virtual void remove(TreeItem* item);
    virtual TreeItem* append(TreeItem* item);
    virtual void kill();
    virtual void kill_children();

    // these are only for Qt
    virtual void emit_begin_insert_rows(int first, int last, std::vector<int>* tree);
    virtual void emit_end_insert_rows();
    virtual void emit_begin_remove_rows(int first, int last, std::vector<int>* tree);
    virtual void emit_end_remove_rows();

    virtual std::string to_string() const;

protected:
    TreeItem* parent_;
    std::vector<TreeItemPtr> children_;
    int ancestor_count_;

    std::vector<TreeItem*> children_weak_;
    bool children_weak_valid_;
};

#endif // TREENODEITEM_H
