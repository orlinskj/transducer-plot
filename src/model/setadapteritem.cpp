#include "setadapteritem.h"
#include "transduceradapteritem.h"

SetAdapterItem::SetAdapterItem(TreeItem* t, const Set *set) :
    TreeItem(t), set_(set) { }

const Set* SetAdapterItem::set()
{
    return set_;
}

int SetAdapterItem::children_count() const
{
    return 0;
}

std::string SetAdapterItem::to_string() const
{
    return set_->unit().symbol();
}
