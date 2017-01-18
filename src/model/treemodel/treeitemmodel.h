#ifndef TREEITEMMODEL_H
#define TREEITEMMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>

#include "treenodeitem.h"

class TreeItemModel : public QAbstractItemModel, public TreeNodeItem
{
public:
    static constexpr int Role = Qt::UserRole + 6729432;

    TreeItemModel(QObject* parent = nullptr);
    virtual ~TreeItemModel();
    //TreeNodeItem* root() const;

    virtual QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QModelIndex parent(const QModelIndex &child) const override;
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;

    void emit_begin_insert_rows(int first, int last, std::vector<int>* tree);
    void emit_end_insert_rows();
    void emit_begin_remove_rows(int first, int last, std::vector<int>* tree);
    void emit_end_remove_rows();

/*protected:
    TreeNodeItemPtr root_;*/
};

Q_DECLARE_METATYPE(TreeNodeItem*)

#endif // PLOTSTOREITEMMODEL_H
