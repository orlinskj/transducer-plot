#ifndef TREEITEMMODEL_H
#define TREEITEMMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>

#include "treeitem.h"

/**
 * @brief The TreeItemModel class
 * @desc Stores items which inherits from TreeItem. It supports both owning and non-owning version of items.
 */
class TreeItemModel : public QAbstractItemModel, public TreeItem
{
public:
    TreeItemModel(QObject* parent = nullptr);
    virtual ~TreeItemModel();

    virtual QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QModelIndex parent(const QModelIndex &child) const override;
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual bool removeRows(int row, int count, const QModelIndex &parent) override;

    void emit_begin_insert_rows(int first, int last, TreeItem* parent) override;
    void emit_end_insert_rows(int first, int last, TreeItem* parent) override;
    void emit_begin_remove_rows(int first, int last, TreeItem* parent) override;
    void emit_end_remove_rows(int first, int last, TreeItem* parent) override;

    QModelIndex index(TreeItem* item) const;
};

Q_DECLARE_METATYPE(TreeItem*)

#endif // PLOTSTOREITEMMODEL_H
