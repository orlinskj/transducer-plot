#include "itemlistview.h"

#include <QMouseEvent>
#include <QDebug>
#include <QPainter>
#include "../viewmodel/treemodel/treeitemmodel.h"

ItemListView::ItemListView(QWidget *parent) :
    QListView(parent),
    delete_button_(new ItemDeleteButton(this))
{
    setMouseTracking(true);
}

void ItemListView::mouseMoveEvent(QMouseEvent *e)
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
}

void ItemListView::mousePressEvent(QMouseEvent *e)
{
    auto index = this->indexAt(e->pos());
    auto selected = this->selectedIndexes().count() > 0 ? this->selectedIndexes().first() : QModelIndex();
    if (index != selected){
        this->selectionModel()->clear();
        if (index != QModelIndex())
            this->setCurrentIndex(index);
        delete_button_->repaint();
    }

    QListView::mousePressEvent(e);
}

void ItemListView::itemEnterEvent(const QModelIndex &index)
{
    delete_button_->show_for_index(index);
}

void ItemListView::itemLeaveEvent(const QModelIndex &index)
{
    delete_button_->hide();
}

void ItemListView::enterEvent(QEvent *e)
{

}

void ItemListView::leaveEvent(QEvent *e)
{
    if (hovered_ != QModelIndex() && hovered_.isValid()){
        itemLeaveEvent(hovered_);
    }
    hovered_ = QModelIndex();
}
