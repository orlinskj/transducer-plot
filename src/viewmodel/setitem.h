#ifndef SETADAPTERITEM_H
#define SETADAPTERITEM_H

#include <string>

#include "treemodel/treeitemt.h"
#include "../model/set.h"



class TransducerItem;

/**
 * @brief The SetItem class
 * @desc Represents transducer set object as TreeItem node.
 */
class SetItem : public TreeItemT<Set>
{
public:
    SetItem(Set *set);
};



#endif // SETADAPTERITEM_H
