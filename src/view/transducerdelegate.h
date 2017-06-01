#ifndef TRANSDUCERDELEGATE_H
#define TRANSDUCERDELEGATE_H

#include <QPainter>
#include <QStyledItemDelegate>

// namespace ac{

/**
 * @brief The TransducerDelegate class
 *
 * Paints transducers in transducer list view.
 */
class TransducerDelegate : public QStyledItemDelegate
{
public:
    TransducerDelegate(QObject* parent = nullptr);

    void paint(QPainter *painter,const QStyleOptionViewItem &option,const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option,const QModelIndex &index) const override;

    static constexpr int transducer_font_size = 11;
    static constexpr int transducer_height = 20;
    static constexpr int transducer_xoffset = 5;

    static constexpr int path_font_size = 7;
    static constexpr int path_height = 10;
    static constexpr int path_xoffset = 7;

    static constexpr int margin_bottom = 2;
};

// } // namespace ac

#endif // TRANSDUCERDELEGATE_H
