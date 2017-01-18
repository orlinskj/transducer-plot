#include "treeitemmodel.h"

TreeItemModel::TreeItemModel(QObject* parent) :
    QAbstractItemModel(parent), TreeNodeItem(nullptr) { }

TreeItemModel::~TreeItemModel() { }

/*TreeNodeItem* TreeItemModel::root() const
{
    return root_.get();
}*/

QModelIndex TreeItemModel::index(int row, int column, const QModelIndex &parent) const
{
    const TreeNodeItem* parent_item;
    if (parent.isValid())
        parent_item = static_cast<TreeNodeItem*>(parent.internalPointer());
    else
        parent_item = this;

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
        if (parent_item == this)
            return QModelIndex();
        int row = parent_item->parent()->child_index(parent_item);
        return createIndex(row,0, (void*)parent_item);
    }

    return QModelIndex();
}

int TreeItemModel::rowCount(const QModelIndex &parent) const
{
    const TreeNodeItem* item;
    if (parent.isValid())
        item = static_cast<TreeNodeItem*>(parent.internalPointer());
    else
        item = this;

    return item->children_count();
}

int TreeItemModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

void TreeItemModel::emit_begin_insert_rows(int first, int last, std::vector<int> *tree)
{
    QModelIndex index = QModelIndex();
    if (tree)
    {
        for(auto it=tree->cbegin(); it!=tree->cend(); it++)
        {
            index = this->index(*it,0,index);
        }
    }
    beginInsertRows(index, first, last);
}

void TreeItemModel::emit_end_insert_rows()
{
    endInsertRows();
}

void TreeItemModel::emit_begin_remove_rows(int first, int last, std::vector<int> *tree)
{
    QModelIndex index = QModelIndex();
    if (tree)
    {
        for(auto it=tree->cbegin(); it!=tree->cend(); it++)
        {
            index = this->index(*it,0,index);
        }
    }
    beginRemoveRows(index, first, last);
}

void TreeItemModel::emit_end_remove_rows()
{
    endRemoveRows();
}
