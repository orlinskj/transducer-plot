#ifndef ITEMLISTVIEW_H
#define ITEMLISTVIEW_H

#include <QListView>
#include "itemdeletebutton.h"

/**
 * @brief The ItemListView class
 *
 * Displays items as list and shows delete button for hovered item.
 */
class ItemListView : public QListView
{
public:
    ItemListView(QWidget* parent = nullptr);

    void show_delete_button(const QModelIndex& index);
    void hide_delete_button();

protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);

    void itemEnterEvent(const QModelIndex& index);
    void itemLeaveEvent(const QModelIndex& index);

protected:
    ItemDeleteButton* delete_button_;

    // current index hovered
    QPersistentModelIndex hovered_;
};

#endif // INTERACTIVELISTVIEW_H

