#ifndef ITEMTREEVIEW_H
#define ITEMTREEVIEW_H

#include <QTreeView>

class ItemDeleteButton;

/**
 * @brief The ItemTreeView class
 * @desc Tree view class with delete button displayed for hovered item.
 */
class ItemTreeView : public QTreeView
{
public:
    ItemTreeView(QWidget* parent = nullptr);

    void show_delete_button(const QModelIndex& index);
    void hide_delete_button();
    void update_index(const QModelIndex& index);

    void rowsInserted(const QModelIndex &parent, int start, int end);

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

#endif // ITEMTREEVIEW_H
