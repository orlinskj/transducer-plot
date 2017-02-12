#ifndef TRANSDUCERDELEGATE_H
#define TRANSDUCERDELEGATE_H

#include <QPainter>
#include <QStyledItemDelegate>

// namespace ac{

class TransducerDelegate : public QStyledItemDelegate
{
public:
    TransducerDelegate(QObject* parent = nullptr);

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index
               ) const override;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index
                   ) const override;
};

// } // namespace ac

#endif // TRANSDUCERDELEGATE_H
