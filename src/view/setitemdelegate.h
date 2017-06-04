#ifndef SETITEMDELEGATE_H
#define SETITEMDELEGATE_H

#include <QStyledItemDelegate>

class SetItemDelegate : public QStyledItemDelegate
{
public:
    SetItemDelegate(const QModelIndex& domain_index, const QModelIndex& codomain_index);

    void paint(QPainter *painter,const QStyleOptionViewItem &option,const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option,const QModelIndex &index) const override;

    static constexpr int caption_font_size = 11;
    static constexpr int desc_font_size = 9;
    static constexpr int set_type_font_size = 9;

protected:
    const QModelIndex& domain_index_;
    const QModelIndex& codomain_index_;
};

#endif // SETITEMDELEGATE_H
