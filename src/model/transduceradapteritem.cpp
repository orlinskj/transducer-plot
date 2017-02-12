#include "transduceradapteritem.h"
#include "setadapteritem.h"

TransducerAdapterItem::TransducerAdapterItem(Transducer* t) :
    TreeItem(nullptr), transducer_(t)
{
    emit_begin_insert_rows(0,t->get_sets().size(),nullptr);
    for(auto it=t->get_sets().cbegin(); it!=t->get_sets().cend(); it++)
    {
        set_adapters_.push_back(SetAdapterItemPtr(new SetAdapterItem(this, &(*it))));
    }
    emit_end_insert_rows();
}

TreeItem* TransducerAdapterItem::child(int index) const
{
    if (index >= 0 && index < int(set_adapters_.size()))
        return set_adapters_[index].get();
    else
        return nullptr;
}

int TransducerAdapterItem::children_count() const
{
    return transducer_->get_sets().size();
}

std::string TransducerAdapterItem::to_string() const
{
    return transducer_->get_name();
}

Transducer* TransducerAdapterItem::transducer() const
{
    return transducer_.get();
}
