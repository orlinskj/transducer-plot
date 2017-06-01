#include "treeitemmodel.h"

TreeItemModel::TreeItemModel(QObject* parent) :
    QAbstractItemModel(parent), TreeItem(nullptr) { }

TreeItemModel::~TreeItemModel() { }

QModelIndex TreeItemModel::index(int row, int column, const QModelIndex &parent) const
{
    const TreeItem* parent_item;
    if (parent.isValid())
        parent_item = static_cast<TreeItem*>(parent.internalPointer());
    else
        parent_item = this;

    if (column == 0)
    {
        auto child_item = parent_item->child(row);
        if (child_item)
            return createIndex(row,column,(void*)child_item);
        else
            return QModelIndex();
    }

    return QModelIndex();
}

QModelIndex TreeItemModel::index(TreeItem *item) const
{
    if (item && item != this)
        return createIndex(item->index(),0,(void*)item);
    else
        return QModelIndex();
}

QVariant TreeItemModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    {
        auto item = static_cast<TreeItem*>(index.internalPointer());
        if (role == Qt::DisplayRole)
            return QVariant(QString::fromStdString(item->to_string()));
        else if (role == TreeItemModel::Role)
            return QVariant::fromValue(item);
    }

    return QVariant();
}

QModelIndex TreeItemModel::parent(const QModelIndex &child) const
{
    qDebug() << "QModelIndex(" << child.row() << child.column() << child.internalPointer() << ").parent()";
    if (child.isValid())
    {
        auto item = static_cast<TreeItem*>(child.internalPointer());

        if (!item || item == this)
            return QModelIndex();

        TreeItem* parent_item = item->parent();
        if (parent_item->is_root())
            return QModelIndex();

        int row = parent_item->index();
        if (row != -1)
            return createIndex(row,0, (void*)parent_item);
    }

    return QModelIndex();
}

int TreeItemModel::rowCount(const QModelIndex &parent) const
{
    const TreeItem* item;
    if (parent.isValid())
        item = static_cast<TreeItem*>(parent.internalPointer());
    else
        item = this;

    return item->children_count();
}

int TreeItemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

bool TreeItemModel::removeRows(int row, int count, const QModelIndex &parent)
{
    TreeItem* parent_item = nullptr;

    if (parent == QModelIndex()){
        parent_item = this;
    }
    else{
        parent_item = static_cast<TreeItem*>(parent.internalPointer());
    }

    std::vector<TreeItem*> removed;

    int curr_row = row;
    while(curr_row-row < count){
        removed.push_back(parent_item->child(curr_row));
        curr_row++;
    }

    for (const auto& item : removed){
        if (item)
            item->kill();
    }

    return true;
}

void TreeItemModel::emit_begin_insert_rows(int first, int last, TreeItem* parent)
{
    auto index = this->index(parent);
    beginInsertRows(index, first, last);
}

void TreeItemModel::emit_end_insert_rows(int first, int last, TreeItem* parent)
{
    endInsertRows();
}

void TreeItemModel::emit_begin_remove_rows(int first, int last, TreeItem* parent)
{
    auto index = this->index(parent);
    beginRemoveRows(index, first, last);
}

void TreeItemModel::emit_end_remove_rows(int first, int last, TreeItem* parent)
{
    endRemoveRows();
}
