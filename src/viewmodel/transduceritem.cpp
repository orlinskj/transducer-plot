#include "transduceritem.h"
#include "setitem.h"

TransducerItem::TransducerItem(Transducer* t) :
    TreeItemTOwner<Transducer>(t)
{
    emit_begin_insert_rows(0,t->get_sets().size(),nullptr);
    for(auto it=t->get_sets().begin(); it!=t->get_sets().end(); it++)
    {
        this->append(new SetItem(&(*it)));
    }
    emit_end_insert_rows();
}
