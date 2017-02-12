#ifndef TRANSDUCERSETPROXYMODEL_H
#define TRANSDUCERSETPROXYMODEL_H

#include "treemodel/flattentreeproxymodel.h"
#include "transduceradapteritem.h"
#include "setadapteritem.h"

// namespace ac{

class TransducerSetProxyModel : public FlattenTreeProxyModel
{
public:
    TransducerSetProxyModel(QObject* parent = nullptr);

    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const;

    void set_transducer_filter(TransducerAdapterItem* t);
    void set_exclude_filter(SetAdapterItem* set);

protected:
    TransducerAdapterItem* transducer_filter_;
    SetAdapterItem* set_filter_;
};

// } // namespace ac

#endif // TRANSDUCERSETPROXYMODEL_H
