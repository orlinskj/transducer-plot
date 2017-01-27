#ifndef TRANSDUCERADAPTERITEM_H
#define TRANSDUCERADAPTERITEM_H

#include "treemodel/treeitem.h"
#include "transducer.h"


namespace ac{

class SetAdapterItem;

using SetAdapterItemPtr = std::unique_ptr<SetAdapterItem>;

class TransducerAdapterItem : public TreeItem
{
public:
    TransducerAdapterItem(Transducer *t);

    TreeItem* child(int index) const override;
    int children_count() const override;

    std::string to_string() const override;

    Transducer* transducer() const;

protected:
    std::vector<SetAdapterItemPtr> set_adapters_;
    std::unique_ptr<Transducer> transducer_;
};

}

#endif // TRANSDUCERADAPTERITEM_H
