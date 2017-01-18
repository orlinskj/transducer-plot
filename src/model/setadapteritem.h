#ifndef SETADAPTERITEM_H
#define SETADAPTERITEM_H

#include <string>

#include "treemodel/treenodeitem.h"
#include "set.h"

namespace ac{

class TransducerAdapterItem;

class SetAdapterItem : public TreeNodeItem
{
public:
    SetAdapterItem(TransducerAdapterItem* parent, const Set *set);

    int children_count() const override;
    std::string to_string() const override;

protected:
    const Set* set_;
};

}

#endif // SETADAPTERITEM_H
