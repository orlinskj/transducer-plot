#include "branchfilterproxymodel.h"
#include "treeitemmodel.h"

BranchFilterProxyModel::BranchFilterProxyModel(QObject* parent) :
    QAbstractProxyModel(parent), branch_root_(nullptr)
{

}

void BranchFilterProxyModel::set_branch(TreeItem *item)
{
    beginResetModel();
    branch_root_ = item;
    endResetModel();
}

QModelIndex BranchFilterProxyModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    auto item = static_cast<TreeItem*>(sourceIndex.internalPointer());

    if (item == branch_root_)
        return QModelIndex();
    else
        return sourceIndex;
}

QModelIndex BranchFilterProxyModel::mapToSource(const QModelIndex &proxyIndex) const
{
    return proxyIndex;
}

int BranchFilterProxyModel::rowCount(const QModelIndex &parent) const
{
    if (!branch_root_)
        return 0;

    if (!parent.isValid())
    {
        if (branch_root_)
            return branch_root_->children_count();
        return 0;
    }
    else
    {
        auto item = static_cast<TreeItem*>(parent.internalPointer());
        return item->children_count();
    }

}

QModelIndex BranchFilterProxyModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        auto item = static_cast<TreeItem*>(parent.internalPointer());
        auto child_item = item->child(row);

        if (child_item)
        {
            return createIndex(row,column,(void*)child_item);
        }
    }
    else
    {
        return createIndex(row,column,(void*)branch_root_);
    }

    return QModelIndex();
}

QModelIndex BranchFilterProxyModel::parent(const QModelIndex &child) const
{
    auto item = static_cast<TreeItem*>(child.internalPointer());
    auto parent = item->parent();

    if (parent && parent->index() >= 0)
    {
        if (parent == branch_root_)
            return QModelIndex();
        return createIndex(parent->index(),0,(void*)parent);
    }
    else
        return QModelIndex();
}

int BranchFilterProxyModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

void BranchFilterProxyModel::setSourceModel(QAbstractItemModel *model)
{
    QAbstractProxyModel::setSourceModel(model);
    auto item_model = dynamic_cast<TreeItemModel*>(model);
    if (item_model)
        set_branch(static_cast<TreeItem*>(item_model));
}

/*QVariant BranchFilterProxyModel::data(const QModelIndex &proxyIndex, int role) const
{
    if (sourceModel())
        return sourceModel()->data(proxyIndex,role);
    else
        return QVariant::fromValue(QString("Not set"));

    //auto item = static_cast<TreeItem*>(proxyIndex.internalPointer());
    //return QVariant::fromValue(QString(item->to_string().c_str()));
}*/
