#ifndef TRANSDUCERSETPROXYMODEL_H
#define TRANSDUCERSETPROXYMODEL_H

#include "treemodel/flattentreeproxymodel.h"
#include "transduceritem.h"
#include "setitem.h"

// namespace ac{

class TransducerSetProxyModel : public FlattenTreeProxyModel
{
public:
    TransducerSetProxyModel(QObject* parent = nullptr);

    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const;

    void set_transducer_filter(TransducerItem* t);
    void set_exclude_filter(SetItem* set);

protected:
    TransducerItem* transducer_filter_;
    SetItem* set_filter_;
};

// } // namespace ac

#endif // TRANSDUCERSETPROXYMODEL_H
