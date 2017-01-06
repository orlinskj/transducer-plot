#ifndef PLOTITEMSDELEGATE_H
#define PLOTITEMSDELEGATE_H

#include <QPainter>
#include <QStyledItemDelegate>

#include "../model/plot.h"

namespace ac{

class PlotItemsDelegate : public QStyledItemDelegate
{
public:
    PlotItemsDelegate(QObject* parent = nullptr);

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index
               ) const override;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index
                   ) const override;
};

}

#endif // PLOTITEMSDELEGATE_H