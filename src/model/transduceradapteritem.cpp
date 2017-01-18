#include "transduceradapteritem.h"
#include "setadapteritem.h"

ac::TransducerAdapterItem::TransducerAdapterItem(ac::Transducer &&t) :
    ac::Transducer(t), TreeNodeItem(nullptr)
{
    for(auto it=get_sets().cbegin(); it!=get_sets().cend(); it++)
    {
        set_adapters_.push_back(ac::SetAdapterItemPtr(new ac::SetAdapterItem(this, &(*it))));
    }
}

TreeNodeItem* ac::TransducerAdapterItem::child(int index) const
{
    if (index >= 0 && index < int(set_adapters_.size()))
        return set_adapters_[index].get();
    else
        return nullptr;
}

int ac::TransducerAdapterItem::children_count() const
{
    return get_sets().size();
}

std::string ac::TransducerAdapterItem::to_string() const
{
    return get_name();
}
