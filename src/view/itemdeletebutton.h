#ifndef ITEMDELETEBUTTON_H
#define ITEMDELETEBUTTON_H

#include <QPushButton>
#include <QPersistentModelIndex>
#include <QAbstractItemView>

/**
 * @brief The ItemDeleteButton class
 *
 * Button displayed by ItemListView and ItemTreeView on top of hovered items.
 */
class ItemDeleteButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(qreal xoffset READ xoffset WRITE setXoffset)

public:
    ItemDeleteButton(QAbstractItemView* view);
    void show_for_index(const QModelIndex& index);

    static constexpr int size = 16;
    static constexpr int icon_size = 8;

    qreal xoffset() const;
    void setXoffset(qreal x);

protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

    QAbstractItemView* view_;
    QPersistentModelIndex index_;
    bool mouse_over_;

    qreal xoffset_;
};

#endif // DELETEPUSHBUTTON_H
