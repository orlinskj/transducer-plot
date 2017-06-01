#include "plotitemdelegate.h"

#include "../viewmodel/treemodel/treeitemmodel.h"
#include "../viewmodel/plotitem.h"
#include "../viewmodel/functionitem.h"
#include "../view/plotpresenter.h"
#include "../viewmodel/plotstoreitemmodel.h"

PlotItemDelegate::PlotItemDelegate(QAbstractItemView* view, PlotPresenter* presenter) :
    QStyledItemDelegate(view), presenter_(presenter), view_(view) { }

void PlotItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    /*auto item = index.data(TreeItem::Role).value<TreeItem*>();
    auto plot_item = dynamic_cast<PlotItem*>(item);

    QFont font = painter->font();
    font.setPointSize(plot_font_size);
    painter->setFont(font);

    QColor function_name_color = option.palette.text().color();
    QColor plot_name_color = option.palette.text().color();

    if (option.state & QStyle::State_Selected){
        painter->fillRect(option.rect, option.palette.highlight().color());
        function_name_color = option.palette.highlightedText().color();
    }

    painter->setPen(plot_name_color);
    painter->drawText(option.rect.topLeft()+QPointF(plot_name_xoffset, plot_name_yoffset), QString::fromStdString(plot_item->to_string()));

    QFont function_font = option.font;
    function_font.setPointSize(function_font_size);
    painter->setFont(function_font);

    painter->setPen(function_name_color);

    int func_name_y = function_name_yoffset + option.rect.top();
    for (const auto& item : plot_item->children()){
        auto func_item = dynamic_cast<FunctionItem*>(item);

        painter->drawText(QPointF(function_name_xoffset, func_name_y), QString::fromStdString(func_item->to_string()));

        func_name_y += function_height;
    }

    painter->setPen(option.palette.alternateBase().color());
    painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());

    // eye icon
    painter->setPen(Qt::black);
    painter->setRenderHint(QPainter::Antialiasing);
    if (presenter_->plot() == plot_item){
        painter->drawPixmap(option.rect.topRight()+QPoint(-40, 6), QPixmap(":/glyphicons/eye-small.png"));

        // drawing circle
        //QPainterPath path;
        //path.addEllipse(option.rect.topRight()+QPoint(-22, 9),3,3);
        //painter->fillPath(path, Qt::black);

    }*/

    QColor text_color = option.palette.text().color();
    QColor text_alt_color = option.palette.text().color();

    auto item = index.data(TreeItem::Role).value<TreeItem*>();
    auto plot_item = dynamic_cast<PlotItem*>(item);

    if (plot_item){
        if (option.state & QStyle::State_Selected){
            painter->fillRect(option.rect, option.palette.highlight().color());
            text_alt_color = option.palette.highlightedText().color();
        }

        painter->setPen(text_color);
        QFont plot_font = option.font;
        plot_font.setPointSize(plot_font_size);

        painter->setFont(plot_font);
        painter->drawText(option.rect.topLeft()+QPointF(plot_name_xoffset, plot_name_yoffset), QString::fromStdString(plot_item->to_string()));

        if (plot_item->children_count() == 0){
            painter->setPen(option.palette.alternateBase().color());
            painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
        }

        if (presenter_->plot() == plot_item){
            painter->drawPixmap(option.rect.topRight()+QPoint(-40, 6), QPixmap(":/glyphicons/eye-small.png"));
        }
    }
    else{
        auto func_item = dynamic_cast<FunctionItem*>(item);

        if (view_->selectionModel()->isSelected(index.parent())){
            painter->fillRect(option.rect, option.palette.highlight().color());
            text_alt_color = option.palette.highlightedText().color();
        }

        painter->setPen(text_alt_color);
        QFont function_font = option.font;
        function_font.setPointSize(function_font_size);
        painter->setFont(function_font);
        painter->drawText(option.rect.topLeft()+QPointF(function_name_xoffset, function_name_yoffset), QString::fromStdString(func_item->to_string()));

        if (func_item->index() == func_item->parent()->children_count() - 1){
            painter->setPen(option.palette.alternateBase().color());
            painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
        }
    }
}

QSize PlotItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    auto item = index.data(TreeItem::Role).value<TreeItem*>();

    if (dynamic_cast<PlotItem*>(item)){
        if (item->children_count() > 0)
            return QSize(option.rect.width(), plot_height);
        else
            return QSize(option.rect.width(), plot_height);
    }
    else{
        if (item->index() < item->parent()->children_count() - 1)
            return QSize(option.rect.width(), function_height);
        else{
            return QSize(option.rect.width(), function_height + margin_bottom);
        }
    }
    /*auto plot_item = PlotItem::from_qmodelindex(index);

    if (plot_item)
        return QSize(0, plot_height + function_height * plot_item->children_count() + margin_bottom);
    else
        return QSize(0, function_height);*/
}
