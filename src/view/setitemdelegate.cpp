#include "setitemdelegate.h"

#include <QPainter>
#include <QStyleOptionViewItem>
#include "../viewmodel/treemodel/treeitemmodel.h"
#include "../viewmodel/setitem.h"

SetItemDelegate::SetItemDelegate(const QModelIndex &domain_index, const QModelIndex &codomain_index) :
    domain_index_(domain_index), codomain_index_(codomain_index)
{

}

void SetItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    auto item = index.data(TreeItem::Role).value<TreeItem*>();
    auto set_item = dynamic_cast<SetItem*>(item);

    QFont caption_font = option.font;
    caption_font.setPointSize(caption_font_size);

    QFont desc_font = option.font;
    desc_font.setPointSize(desc_font_size);

    QFont set_type_font = option.font;
    set_type_font.setPointSize(set_type_font_size);

    QColor text_color = option.palette.text().color();
    QColor text_alt_color = option.palette.text().color();
    QColor text_mid_color = option.palette.mid().color();
    QColor text_mid_alt_color = option.palette.mid().color();

    if (option.state & QStyle::State_Selected){
        //painter->fillRect(option.rect, option.palette.highlight().color());
        text_alt_color = option.palette.highlightedText().color();
        text_mid_alt_color = option.palette.highlightedText().color();
    }

    QPoint caption_start = option.rect.topLeft() + QPoint(30, caption_font_size + 3);

    if (index.row() == domain_index_.row() || index.row() == codomain_index_.row()){
        //if (!(option.state & QStyle::State_Selected)){
            painter->fillRect(option.rect, QColor(255,220,255));
            //painter->fillRect(option.rect, option.palette.shadow().color());
        //}

        QPoint set_type_start = option.rect.topLeft() + QPoint(10, set_type_font_size + 3);
        QString set_text = index.row() == domain_index_.row() ? tr("(dziedzina)") : tr("(przeciwdziedzina)");
        painter->setFont(set_type_font);
        painter->setPen(text_color);
        painter->drawText(set_type_start,set_text);

        caption_start.ry() += set_type_start.y() - option.rect.topLeft().y() + 3;
        caption_start.rx() = set_type_start.x();
    }

    painter->setFont(caption_font);
    painter->setPen(text_color);
    QString symbol = QString::fromStdString(set_item->value()->unit().symbol());
    painter->drawText(caption_start, symbol);

    QFontMetrics caption_fm(caption_font);
    QPoint desc_start = caption_start + QPoint(caption_fm.width(symbol) + 10, 0);
    painter->setFont(desc_font);
    painter->setPen(text_mid_color);
    painter->drawText(desc_start, QString::fromStdString(set_item->value()->unit().name()));
}

QSize SetItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index == codomain_index_ || index == domain_index_){
        return QSize(0, caption_font_size + 6 + set_type_font_size + 6);
    }

    return QSize(0, caption_font_size + 6);
}
