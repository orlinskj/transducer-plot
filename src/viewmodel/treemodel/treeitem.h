#ifndef TREENODEITEM_H
#define TREENODEITEM_H


#include <vector>
#include <string>
#include <memory>

#include <QtCore>

class TreeItem;

using TreeItem_ptr = std::unique_ptr<TreeItem>;

/**
 * @brief The TreeItem class
 * @desc Tree node item.
 */
class TreeItem
{
public:
    static constexpr int Role = Qt::UserRole + 1;

    TreeItem(TreeItem *parent = nullptr);
    virtual ~TreeItem();

    int children_count() const;
    int ancestor_count() const;

    int child_index(const TreeItem *child) const;
    int index() const;
    int absolute_index(TreeItem* relative=nullptr) const;
    const std::vector<TreeItem*>& children();

    TreeItem* child(int index) const;
    TreeItem* parent() const;
    const TreeItem* absolute_child(int index) const;
    void path_to_root(std::vector<TreeItem*>* path) const;
    bool is_root() const;
    TreeItem* root();

    virtual void remove(TreeItem* item);
    virtual TreeItem* append(TreeItem* item);
    void kill();
    void kill_children();

    // signals can be used in subclasses for framework specific signalling
    virtual void emit_begin_insert_rows(int first, int last, TreeItem* parent);
    virtual void emit_end_insert_rows(int first, int last, TreeItem* parent);
    virtual void emit_begin_remove_rows(int first, int last, TreeItem* parent);
    virtual void emit_end_remove_rows(int first, int last, TreeItem* parent);

    virtual std::string to_string() const;

    template <typename T>
    static T* from_qmodelindex(const QModelIndex& index);

protected:
    TreeItem* parent_;
    std::vector<TreeItem*> children_;
    int ancestor_count_; // cached
};

template <typename T>
T* TreeItem::from_qmodelindex(const QModelIndex& index)
{
    return dynamic_cast<T*>(index.data(TreeItem::Role).value<TreeItem*>());
}

#endif // TREENODEITEM_H
