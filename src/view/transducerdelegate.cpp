#include "transducerdelegate.h"

#include "../model/transducer.h"
#include "../viewmodel/treemodel/treeitemmodel.h"
#include "../viewmodel/transduceritem.h"

TransducerDelegate::TransducerDelegate(QObject* parent) :
    QStyledItemDelegate(parent) { }

void TransducerDelegate::paint(QPainter *p, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    auto tree_item = index.data(TreeItemModel::Role).value<TreeItem*>();
    auto transducer_item = dynamic_cast<TransducerItem*>(tree_item);
    auto transducer = transducer_item->value();

    QString name  = transducer->get_name().c_str();
    QString path = transducer->get_source().c_str();

    QFont small_font = QFont(option.font);
    small_font.setPointSize(8);

    if (option.state & QStyle::State_Selected)
        p->fillRect(option.rect, option.palette.highlight());

    p->setPen(option.palette.text().color());
    p->setFont(option.font);
    p->drawText(option.rect.adjusted(2,0,0,0),Qt::AlignLeft, name);

    if (option.state & QStyle::State_Selected)
        p->setPen(option.palette.highlightedText().color());
    else
        p->setPen(option.palette.mid().color());

    p->setFont(small_font);
    p->drawText(option.rect.adjusted(2,16,0,0), Qt::AlignLeft, path);

}

QSize TransducerDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    //return QSize(option.rect.width(), option.decorationSize.height()+8);
    return QSize(option.rect.width(), 32);
}
