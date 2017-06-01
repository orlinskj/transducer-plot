#ifndef FLATTENTREEPROXYMODEL_H
#define FLATTENTREEPROXYMODEL_H

#include <QAbstractProxyModel>
#include "treeitem.h"
#include "treeitemmodel.h"

/// NOTE: this class is currently not used in code!
/// NOTE: this class is not working properly yet!

/**
 * @brief The FlattenTreeProxyModel class
 * @desc Proxy model which flats tree model to a flat list model.
 */
class FlattenTreeProxyModel : public QAbstractProxyModel
{
public:
    FlattenTreeProxyModel(QObject* parent = nullptr);

    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;

    QModelIndex parent(const QModelIndex &child) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    void setSourceModel(QAbstractItemModel* model) override;
protected:
    TreeItemModel* root_;
};

#endif // FLATTENTREEPROXYMODEL_H
