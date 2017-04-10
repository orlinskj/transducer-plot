#ifndef PLOTITEMSDELEGATE_H
#define PLOTITEMSDELEGATE_H

#include <QPainter>
#include <QStyledItemDelegate>

#include "../model/plot.h"

// namespace ac{

class PlotItemDelegate : public QStyledItemDelegate
{
public:
    PlotItemDelegate(QObject* parent = nullptr);

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index
               ) const override;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index
                   ) const override;
};

// } // namespace ac

#endif // PLOTITEMSDELEGATE_H
