#include "transduceradapteritem.h"
#include "setadapteritem.h"

ac::TransducerAdapterItem::TransducerAdapterItem(ac::Transducer* t) :
    TreeItem(nullptr), transducer_(t)
{
    emit_begin_insert_rows(0,t->get_sets().size(),nullptr);
    for(auto it=t->get_sets().cbegin(); it!=t->get_sets().cend(); it++)
    {
        set_adapters_.push_back(ac::SetAdapterItemPtr(new ac::SetAdapterItem(this, &(*it))));
    }
    emit_end_insert_rows();
}

TreeItem* ac::TransducerAdapterItem::child(int index) const
{
    if (index >= 0 && index < int(set_adapters_.size()))
        return set_adapters_[index].get();
    else
        return nullptr;
}

int ac::TransducerAdapterItem::children_count() const
{
    return transducer_->get_sets().size();
}

std::string ac::TransducerAdapterItem::to_string() const
{
    return transducer_->get_name();
}

ac::Transducer* ac::TransducerAdapterItem::transducer() const
{
    return transducer_.get();
}
