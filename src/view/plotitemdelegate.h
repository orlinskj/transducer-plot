#ifndef PLOTITEMSDELEGATE_H
#define PLOTITEMSDELEGATE_H

#include <QPainter>
#include <QStyledItemDelegate>
#include <QAbstractItemView>

#include "../model/plot.h"

// namespace ac{

class PlotPresenter;

class PlotItemDelegate : public QStyledItemDelegate
{
public:
    static constexpr int margin_bottom = 3;

    static constexpr int plot_font_size = 11;
    static constexpr int plot_height = 22;

    static constexpr int plot_name_xoffset = 10;
    static constexpr int plot_name_yoffset = plot_font_size + 5;

    static constexpr int function_font_size = 9;
    static constexpr int function_height = 15;
    static constexpr int function_name_xoffset = 25;
    static constexpr int function_name_yoffset = function_font_size + 3;

    PlotItemDelegate(QAbstractItemView* view, PlotPresenter* presenter);

    void paint(QPainter *painter,const QStyleOptionViewItem &option,const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option,const QModelIndex &index) const override;

protected:
    PlotPresenter* presenter_;
    QAbstractItemView* view_;
};

// } // namespace ac

#endif // PLOTITEMSDELEGATE_H
