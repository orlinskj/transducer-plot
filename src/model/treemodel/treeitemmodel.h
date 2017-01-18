#ifndef TREEITEMMODEL_H
#define TREEITEMMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>

#include "treenodeitem.h"

class TreeItemModel : public QAbstractItemModel
{
public:
    static constexpr int Role = Qt::UserRole + 6729432;

    TreeItemModel();
    virtual ~TreeItemModel();
    TreeNodeItem* root() const;

    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;

protected:
    TreeNodeItemPtr root_;
};

Q_DECLARE_METATYPE(TreeNodeItem*)

#endif // PLOTSTOREITEMMODEL_H
