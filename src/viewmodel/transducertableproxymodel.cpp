#include "transducertableproxymodel.h"

#include <algorithm>
#include "../viewmodel/setitem.h"

TransducerTableProxyModel::TransducerTableProxyModel(TransducerItem* t):
    transducer_(t)
{
}

int TransducerTableProxyModel::columnCount(const QModelIndex &parent) const
{
    if (parent == QModelIndex())
        return transducer_->children_count();
    else
        return 0;
}

int TransducerTableProxyModel::rowCount(const QModelIndex &parent) const
{
    if (parent == QModelIndex())
    {
        if (transducer_->children_count() > 0)
        {
            auto sets = transducer_->children();
            auto it = std::max_element(sets.cbegin(), sets.cend(),
                [](const auto& s1, const auto& s2){
                    auto s1c = dynamic_cast<SetItem*>(s1);
                    auto s2c = dynamic_cast<SetItem*>(s2);
                    return s1c->value()->values().size() < s2c->value()->values().size();
                });
            return dynamic_cast<SetItem*>(*it)->value()->values().size();
        }
    }

    return 0;
}

QModelIndex TransducerTableProxyModel::mapToSource(const QModelIndex &proxyIndex) const
{
    Q_UNUSED(proxyIndex);
    return QModelIndex();
}

QModelIndex TransducerTableProxyModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    Q_UNUSED(sourceIndex);
    return QModelIndex();
}

QModelIndex TransducerTableProxyModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent != QModelIndex())
    {
        return QModelIndex();
    }

    auto set = dynamic_cast<SetItem*>(transducer_->child(column));
    return createIndex(row,column,(void*)&(set->value()->values().at(row)));
}

QVariant TransducerTableProxyModel::data(const QModelIndex &proxyIndex, int role) const
{
    if (role == Qt::DisplayRole)
    {
        SetType val = *static_cast<SetType*>(proxyIndex.internalPointer());
        return QVariant::fromValue(val);
    }

    return QAbstractProxyModel::data(proxyIndex,role);

}

QModelIndex TransducerTableProxyModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return QModelIndex();
}

QVariant TransducerTableProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QAbstractProxyModel::headerData(section,orientation,role);

    if (orientation == Qt::Vertical)
        return QVariant::fromValue(section + 1);
    else
    {
        if (size_t(section) < transducer_->value()->get_sets().size())
            return QString::fromStdString(transducer_->value()->get_sets().at(section).unit().name());
        else
            return QVariant();
    }
}

bool TransducerTableProxyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return false;
}

Qt::ItemFlags TransducerTableProxyModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEditable | Qt::ItemIsSelectable;
}
