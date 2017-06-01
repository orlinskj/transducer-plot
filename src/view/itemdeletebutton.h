#ifndef ITEMDELETEBUTTON_H
#define ITEMDELETEBUTTON_H

#include <QPushButton>
#include <QPersistentModelIndex>
#include <QAbstractItemView>

class ItemView;

class ItemDeleteButton : public QPushButton
{
public:
    ItemDeleteButton(QAbstractItemView* view);
    void show_for_index(const QModelIndex& index);

    static constexpr int size = 16;
    static constexpr int icon_size = 8;

protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

    QAbstractItemView* view_;
    QPersistentModelIndex index_;
    bool mouse_over_;
};

#endif // DELETEPUSHBUTTON_H
