#ifndef PLOTSTOREITEM_H
#define PLOTSTOREITEM_H

#include <QtCore>

namespace ac{

class PlotStoreItem
{
public:
    static constexpr int Role = Qt::UserRole + 1;

    virtual ~PlotStoreItem();

    virtual std::string to_string() const = 0;
};

}

#endif // PLOTSTOREITEM_H
