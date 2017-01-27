#include "flattentreeproxymodel.h"
#include "treeitem.h"
#include "treeitemmodel.h"

FlattenTreeProxyModel::FlattenTreeProxyModel(QObject *parent) :
    QAbstractProxyModel(parent), root_(nullptr) { }

QModelIndex FlattenTreeProxyModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    auto tree_item = static_cast<TreeItem*>(sourceIndex.internalPointer());

    if (!tree_item->parent())
        return QModelIndex();

    return createIndex(tree_item->absolute_index()-1,0,sourceIndex.internalPointer());
}

QModelIndex FlattenTreeProxyModel::mapToSource(const QModelIndex &proxyIndex) const
{
    if (proxyIndex != QModelIndex())
    {
        auto item = static_cast<TreeItem*>(proxyIndex.internalPointer());
        return createIndex(item->index(),0,proxyIndex.internalPointer());
    }
    else
    {
        return QModelIndex();
    }
}

QModelIndex FlattenTreeProxyModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

QModelIndex FlattenTreeProxyModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent == QModelIndex() && root_)
    {
        auto item = root_->absolute_child(row+1);
        return createIndex(row, 0, (void*)item);
    }
    return QModelIndex();
}

void FlattenTreeProxyModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    QAbstractProxyModel::setSourceModel(sourceModel);
    root_ = dynamic_cast<TreeItemModel*>(sourceModel);
}

int FlattenTreeProxyModel::rowCount(const QModelIndex &parent) const
{
    if (parent == QModelIndex() && root_)
        return root_->ancestor_count();
    else
        return 0;
}

int FlattenTreeProxyModel::columnCount(const QModelIndex &parent) const
{
    if (parent == QModelIndex() && root_)
        return 1;
    else
        return 0;
}
