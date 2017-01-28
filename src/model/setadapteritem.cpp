#include "setadapteritem.h"
#include "transduceradapteritem.h"

ac::SetAdapterItem::SetAdapterItem(TreeItem* t, const Set *set) :
    TreeItem(t), set_(set) { }

const Set* ac::SetAdapterItem::set()
{
    return set_;
}

int ac::SetAdapterItem::children_count() const
{
    return 0;
}

std::string ac::SetAdapterItem::to_string() const
{
    return set_->unit().symbol();
}
