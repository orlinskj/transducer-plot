#include "transduceritem.h"
#include "setitem.h"

TransducerItem::TransducerItem(Transducer* t) :
    TreeItemTOwner<Transducer>(t)
{
    for(auto& set : t->get_sets()){
        this->append(new SetItem(&set));
    }
}
