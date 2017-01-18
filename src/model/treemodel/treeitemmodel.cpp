#include "treeitemmodel.h"

TreeItemModel::TreeItemModel() : root_(new TreeNodeItem()) { }

TreeItemModel::~TreeItemModel()
{
    //delete root_;
}

TreeNodeItem* TreeItemModel::root() const
{
    return root_.get();
}

QModelIndex TreeItemModel::index(int row, int column, const QModelIndex &parent) const
{
    TreeNodeItem* parent_item;
    if (parent.isValid())
        parent_item = static_cast<TreeNodeItem*>(parent.internalPointer());
    else
        parent_item = root_.get();

    if (column == 0)
    {
        auto child_item = parent_item->child(row);
        if (child_item)
            return createIndex(row,column,(void*)child_item);
        else
            return QModelIndex();
    }

    return QModelIndex();
}

QVariant TreeItemModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    {
        auto item = static_cast<TreeNodeItem*>(index.internalPointer());
        if (role == Qt::DisplayRole)
            return QVariant(QString::fromStdString(item->to_string()));
        else if (role == TreeItemModel::Role)
            return QVariant::fromValue(item);
    }

    return QVariant();
}

QModelIndex TreeItemModel::parent(const QModelIndex &child) const
{
    if (child.isValid())
    {
        auto parent_item = static_cast<TreeNodeItem*>(child.internalPointer())->parent();
        if (parent_item == root())
            return QModelIndex();
        int row = parent_item->parent()->child_index(parent_item);
        return createIndex(row,0, (void*)parent_item);
    }

    return QModelIndex();
}

int TreeItemModel::rowCount(const QModelIndex &parent) const
{
    TreeNodeItem* item;
    if (parent.isValid())
        item = static_cast<TreeNodeItem*>(parent.internalPointer());
    else
        item = root_.get();

    return item->children_count();
}

int TreeItemModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}
