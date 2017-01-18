#ifndef MODEL_PLOTSTORE_H
#define MODEL_PLOTSTORE_H

#include <QAbstractItemModel>
#include <vector>

#include "plot.h"
#include "treemodel/treeitemmodel.h"

namespace ac{

class PlotStoreItemModel : public TreeItemModel
{
Q_OBJECT

public:
    PlotStoreItemModel(QObject *parent = nullptr);
    virtual ~PlotStoreItemModel() override;

protected:

};

}

#endif // PLOTSTORE_H
