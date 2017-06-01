#include "itemtreeview.h"

#include <QMouseEvent>
#include <QDebug>
#include <QPainter>
#include "../viewmodel/treemodel/treeitemmodel.h"
#include "../viewmodel/functionitem.h"
#include "itemdeletebutton.h"

ItemTreeView::ItemTreeView(QWidget *parent) :
    QTreeView(parent), delete_button_(new ItemDeleteButton(this))
{
    setHeaderHidden(true);
    setMouseTracking(true);
}

void ItemTreeView::mouseMoveEvent(QMouseEvent *e)
{
    auto index = this->indexAt(e->pos());
    if (index != hovered_){
        if (hovered_ != QModelIndex() && hovered_.isValid()){
            itemLeaveEvent(hovered_);
        }
        if (index != QModelIndex()){
            itemEnterEvent(index);
        }
        hovered_ = index;
    }

    // QTreeView::mouseMoveEvent(e);
}

void ItemTreeView::rowsInserted(const QModelIndex &parent, int start, int end)
{
    qDebug() << "rows inserted";
    QTreeView::rowsInserted(parent,start,end);
    auto index = this->model()->index(start,0,parent);
    expand(index);
}

void ItemTreeView::update_index(const QModelIndex& index)
{
    auto row_count = model()->rowCount(index);

    for (int i=0; i<row_count; i++){
        update(model()->index(i,0,index));
    }
}

void ItemTreeView::mousePressEvent(QMouseEvent *e)
{
    auto prev = this->selectedIndexes().count() > 0 ? this->selectedIndexes().first() : QModelIndex();
    // should be at end of method ??
    QAbstractItemView::mousePressEvent(e);

    // auto index = this->indexAt(e->pos());
    auto selected = this->selectedIndexes().count() > 0 ? this->selectedIndexes().first() : QModelIndex();

    if (dynamic_cast<FunctionItem*>(selected.data(TreeItem::Role).value<TreeItem*>())){
        selected = selected.parent();
    }

    if (indexAt(e->pos()) == QModelIndex())
        selectionModel()->clear();
    else{
        setCurrentIndex(selected);
        update_index(selected);
    }

    if (prev != QModelIndex())
        update_index(prev);
    delete_button_->repaint();
}

void ItemTreeView::itemEnterEvent(const QModelIndex &index)
{
    delete_button_->show_for_index(index);
}

void ItemTreeView::itemLeaveEvent(const QModelIndex &index)
{
    delete_button_->hide();
}

void ItemTreeView::enterEvent(QEvent *e)
{
    QTreeView::enterEvent(e);
}

void ItemTreeView::leaveEvent(QEvent *e)
{
    if (hovered_ != QModelIndex() && hovered_.isValid()){
        itemLeaveEvent(hovered_);
    }
    hovered_ = QModelIndex();

    QTreeView::leaveEvent(e);
}
