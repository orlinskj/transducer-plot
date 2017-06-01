#include "itemdeletebutton.h"

#include <QPainter>

ItemDeleteButton::ItemDeleteButton(QAbstractItemView *view) :
    QPushButton(view), view_(view), mouse_over_(false)
{
    constexpr int size = 16;
    constexpr int icon_size = 8;

    resize(size,size);
    setIconSize(QSize(icon_size,icon_size));

    setIcon(QIcon(":/glyphicons/glyphicons-208-remove.png"));
    setStyleSheet("border: 0;");
    setMouseTracking(true);

    hide();
}

void ItemDeleteButton::show_for_index(const QModelIndex &index)
{
    index_ = index;
    if (index_ != QModelIndex())
        show();
    repaint();
}

void ItemDeleteButton::mousePressEvent(QMouseEvent *e)
{
    if (index_ != QModelIndex()){
        view_->model()->removeRows(index_.row(),1,index_.parent());
    }

    auto index = view_->indexAt(pos());
    this->show_for_index(index);
}

void ItemDeleteButton::paintEvent(QPaintEvent *e)
{
    if (index_ == QModelIndex())
        return;

    // moving to wanted position
    auto rect = view_->visualRect(index_);
    auto y = (rect.height() - 16) / 2;

    if (index_.parent() == QModelIndex())
        this->move(rect.topRight() + QPoint(-16,y));
    else{
        this->move(rect.topRight() + QPoint(-38,y));
    }

    if (mouse_over_){
        setIcon(QIcon(":/glyphicons/remove-red.png"));
    }
    else if (view_->currentIndex() == index_ || (view_->currentIndex() == index_.parent() && index_.parent() != QModelIndex())){
        setIcon(QIcon(":/glyphicons/remove-white.png"));
    }
    else{
        setIcon(QIcon(":/glyphicons/remove-grey.png"));
    }

    QPushButton::paintEvent(e);
    QPainter p(this);
}

void ItemDeleteButton::enterEvent(QEvent *event)
{
    mouse_over_ = true;
    repaint();
}

void ItemDeleteButton::leaveEvent(QEvent *event)
{
    mouse_over_ = false;
    repaint();
}
