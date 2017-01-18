#include "setadapteritem.h"
#include "transduceradapteritem.h"

ac::SetAdapterItem::SetAdapterItem(ac::TransducerAdapterItem* t, const Set *set) :
    TreeNodeItem(t), set_(set) { }

int ac::SetAdapterItem::children_count() const
{
    return 0;
}

std::string ac::SetAdapterItem::to_string() const
{
    return set_->get_unit().get_symbol();
}
