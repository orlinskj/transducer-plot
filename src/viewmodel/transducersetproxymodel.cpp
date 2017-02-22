#include "transducersetproxymodel.h"

TransducerSetProxyModel::TransducerSetProxyModel(QObject* parent) :
    FlattenTreeProxyModel(parent),
    transducer_filter_(nullptr), set_filter_(nullptr)
{

}

void TransducerSetProxyModel::set_exclude_filter(SetItem *set)
{
    set_filter_ = set;
}

void TransducerSetProxyModel::set_transducer_filter(TransducerItem *t)
{
    transducer_filter_ = t;
}

QModelIndex TransducerSetProxyModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    Q_UNUSED(sourceIndex);
    /*if (!transducer_filter_)
        return QModelIndex();

    auto item = static_cast<TreeNodeItem*>(sourceIndex.internalPointer());*/

    //int index = FlattenTreeProxyModel::index_in_parent(item);
    /*int parent
    if (index < */
    return QModelIndex();
}
