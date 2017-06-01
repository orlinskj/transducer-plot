#ifndef BRANCHFILTERPROXYMODEL_H
#define BRANCHFILTERPROXYMODEL_H

#include <QAbstractProxyModel>
#include "treeitem.h"
#include <vector>

/// NOTE: this class is currently not used in code!
/// NOTE: this class is not working properly yet!

/**
 * @brief The BranchFilterProxyModel class
 * @desc Filters model to specified branch only.
 */
class BranchFilterProxyModel : public QAbstractProxyModel
{
public:
    BranchFilterProxyModel(QObject* parent = nullptr);

    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;

    QModelIndex parent(const QModelIndex &child) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    void setSourceModel(QAbstractItemModel* model) override;

    //QVariant data(const QModelIndex &proxyIndex, int role) const;

    void set_branch(TreeItem* item);
protected:
    TreeItem* branch_root_;
};

#endif // BRANCHFILTERPROXYMODEL_H
