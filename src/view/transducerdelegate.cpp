#include "transducerdelegate.h"

#include "../model/transducer.h"
#include "../model/treemodel/treeitemmodel.h"
#include "../model/transduceradapteritem.h"

ac::TransducerDelegate::TransducerDelegate(QObject* parent) :
    QStyledItemDelegate(parent) { }

void ac::TransducerDelegate::paint(
        QPainter *p,
        const QStyleOptionViewItem &option,
        const QModelIndex &index
        ) const
{
    auto tree_item = index.data(TreeItemModel::Role).value<TreeItem*>();
    /*if (auto transducer_adapter_item = dynamic_cast<ac::TransducerAdapterItem*>(tree_item))
    {*/
        auto transducer_adapter_item = dynamic_cast<ac::TransducerAdapterItem*>(tree_item);
        auto transducer = transducer_adapter_item->transducer();

        //auto transducer = index.data(ac::Transducer::Role).value<ac::Transducer_ptr>();

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
    /*}*/

}

QSize ac::TransducerDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(option.rect.width(), option.decorationSize.height()+8);
}
