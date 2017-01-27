#ifndef SETADAPTERITEM_H
#define SETADAPTERITEM_H

#include <string>

#include "treemodel/treeitem.h"
#include "set.h"

namespace ac{

class TransducerAdapterItem;

class SetAdapterItem : public TreeItem
{
public:
    SetAdapterItem(TreeItem *parent, const Set *set);

    int children_count() const override;
    std::string to_string() const override;
    const Set* set();

protected:
    const Set* set_;
};

}

#endif // SETADAPTERITEM_H
