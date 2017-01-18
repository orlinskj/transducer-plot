#ifndef PLOTSTOREITEM_H
#define PLOTSTOREITEM_H

#include <QtCore>

namespace ac{

class PlotStoreItemModel;

class PlotStoreItem
{
public:
    static constexpr int Role = Qt::UserRole + 1;

    PlotStoreItem();
    virtual ~PlotStoreItem();

    virtual std::string to_string() const = 0;

protected:
    PlotStoreItemModel* model_;
};

}

#endif // PLOTSTOREITEM_H
