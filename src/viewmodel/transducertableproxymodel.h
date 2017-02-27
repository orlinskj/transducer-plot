#ifndef TRANSDUCERTABLEPROXYMODEL_H
#define TRANSDUCERTABLEPROXYMODEL_H

#include <QAbstractProxyModel>
#include "../viewmodel/transduceritem.h"

class TransducerTableProxyModel : public QAbstractProxyModel
{
public:
    TransducerTableProxyModel(TransducerItem* t);

    void transducer() const;

    QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;
    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;
    int columnCount(const QModelIndex &parent) const override;
    int rowCount(const QModelIndex &parent) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    QVariant data(const QModelIndex &proxyIndex, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    TransducerItem* transducer_;
};

#endif // TRANSDUCERTABLEPROXYMODEL_H
