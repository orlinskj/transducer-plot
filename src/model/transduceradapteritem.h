#ifndef TRANSDUCERADAPTERITEM_H
#define TRANSDUCERADAPTERITEM_H

#include "treemodel/treenodeitem.h"
#include "transducer.h"


namespace ac{

class SetAdapterItem;

using SetAdapterItemPtr = std::unique_ptr<SetAdapterItem>;

class TransducerAdapterItem : public Transducer, public TreeNodeItem
{
public:
    TransducerAdapterItem(Transducer&& t);

    TreeNodeItem* child(int index) const override;
    int children_count() const override;

    std::string to_string() const override;

protected:
    std::vector<SetAdapterItemPtr> set_adapters_;
};

}

#endif // TRANSDUCERADAPTERITEM_H
