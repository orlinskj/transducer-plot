#ifndef VIEWMODEL_TRANSDUCERITEM_H
#define VIEWMODEL_TRANSDUCERITEM_H

#include "treemodel/treeitemt.h"
#include "../model/transducer.h"

/**
 * @brief The TransducerItem class
 *
 * Adapter for Transduer class which allows to store it in TreeItemModel.
 */
class TransducerItem : public TreeItemTOwner<Transducer>
{
public:
    TransducerItem(Transducer *t = nullptr);
};



#endif // VIEWMODEL_TRANSDUCERITEM_H
