#include "plotitemsdelegate.h"

#include <QDebug>

PlotItemsDelegate::PlotItemsDelegate(QObject* parent) :
    QStyledItemDelegate(parent) { }

void PlotItemsDelegate::paint(QPainter *painter,
                               const QStyleOptionViewItem &option,
                               const QModelIndex &index) const
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(index);
    /*if (index.data(PlotStoreItem::Role).canConvert<Plot*>())
    {
        auto plot = index.data(PlotStoreItem::Role).value<Plot*>();

        painter->setPen(option.palette.text().color());

        if (option.state & QStyle::State_Selected)
        {
            painter->fillRect(option.rect, option.palette.highlight());

            int top = option.rect.top();
            int right = option.rect.right();
            QPixmap plus = QPixmap(":/glyphicons/glyphicons-433-plus.png");
            painter->drawPixmap(right-18, top+3, 12, 12, plus);

            painter->setPen(option.palette.highlightedText().color());
        }

        painter->setFont(option.font);
        painter->drawText(option.rect,Qt::AlignLeft, plot->get_name().c_str());


    }
    else if (index.data(PlotStoreItem::Role).canConvert<Function*>())
    {
        QStyledItemDelegate::paint(painter, option, index);
    }*/
    //QStyledItemDelegate::paint(painter, option, index);
}

QSize PlotItemsDelegate::sizeHint(const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const
{
    /*if (index.data(PlotStoreItem::Role).canConvert<Plot*>())
    {
        auto plot = index.data(PlotStoreItem::Role).value<Plot*>();

        if (option.state & QStyle::State_Children)
        {
            QSize size = QStyledItemDelegate::sizeHint(option,index);
            size.rheight() += size.height() * plot->function_count();
            return size;
        }
        else
        {
            return QStyledItemDelegate::sizeHint(option,index);
        }
    }*/

    return QStyledItemDelegate::sizeHint(option,index);
}
