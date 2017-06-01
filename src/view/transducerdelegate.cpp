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

    QFont transducer_font = QFont(option.font);
    transducer_font.setPointSize(transducer_font_size);
    QColor transducer_color = option.palette.text().color();

    QFont path_font = QFont(option.font);
    path_font.setPointSize(path_font_size);
    QColor path_color = option.palette.mid().color();

    if (option.state & QStyle::State_Selected){
        p->fillRect(option.rect, option.palette.highlight());
        path_color = option.palette.highlightedText().color();
    }
    else if(option.state & QStyle::State_MouseOver){
        // this works only on windows for now (?)
    }

    p->setPen(transducer_color);
    p->setFont(transducer_font);

    constexpr int t_yoffset = transducer_font_size + (transducer_height - transducer_font_size)/2;
    QPoint t_start = option.rect.topLeft() + QPoint(transducer_xoffset, t_yoffset);
    p->drawText(t_start, name);

    p->setFont(path_font);
    p->setPen(path_color);

    constexpr int p_yoffset = transducer_height + path_font_size + (path_height - path_font_size)/2;
    QPoint p_start = option.rect.topLeft() + QPoint(path_xoffset, p_yoffset);
    p->drawText(p_start, path);

    p->setPen(option.palette.alternateBase().color());
    p->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
}

QSize TransducerDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    return QSize(option.rect.width(), transducer_height + path_height + margin_bottom);
}
