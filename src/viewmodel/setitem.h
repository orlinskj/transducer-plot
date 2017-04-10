#ifndef SETADAPTERITEM_H
#define SETADAPTERITEM_H

#include <string>

#include "treemodel/treeitemt.h"
#include "../model/set.h"



class TransducerItem;

class SetItem : public TreeItemT<Set>
{
public:
    SetItem(Set *set);
};



#endif // SETADAPTERITEM_H
