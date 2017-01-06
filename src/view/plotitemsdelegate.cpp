#include "plotitemsdelegate.h"

ac::PlotItemsDelegate::PlotItemsDelegate(QObject* parent) :
    QStyledItemDelegate(parent) { }

void ac::PlotItemsDelegate::paint(QPainter *painter,
                               const QStyleOptionViewItem &option,
                               const QModelIndex &index) const
{
    if (index.data(ac::Plot::Role).canConvert<ac::Plot_ptr>())
    {
        ac::Plot_ptr plot = index.data(ac::Plot::Role).value<ac::Plot_ptr>();

        QString name = plot->get_name().c_str();

        painter->setPen(option.palette.text().color());
        painter->setFont(option.font);
        painter->drawText(option.rect,Qt::AlignLeft, name);
    }
    else if (index.data(ac::Function::Role).canConvert<ac::Function>())
    {

    }




    QStyledItemDelegate::paint(painter, option, index);
}

QSize ac::PlotItemsDelegate::sizeHint(const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const
{
    return QStyledItemDelegate::sizeHint(option,index);
}
