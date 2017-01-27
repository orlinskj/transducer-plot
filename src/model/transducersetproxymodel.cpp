#include "transducersetproxymodel.h"

TransducerSetProxyModel::TransducerSetProxyModel(QObject* parent) :
    transducer_filter_(nullptr), set_filter_(nullptr)
{

}

void ac::TransducerSetProxyModel::set_exclude_filter(SetAdapterItem *set)
{
    set_filter_ = set;
}

void ac::TransducerSetProxyModel::set_transducer_filter(TransducerAdapterItem *t)
{
    transducer_filter_ = t;
}

QModelIndex ac::TransducerSetProxyModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    /*if (!transducer_filter_)
        return QModelIndex();

    auto item = static_cast<TreeNodeItem*>(sourceIndex.internalPointer());*/

    //int index = FlattenTreeProxyModel::index_in_parent(item);
    /*int parent
    if (index < */
    return QModelIndex();
}
