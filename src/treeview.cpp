#include "treeview.h"

using namespace ac;

TreeView::TreeView(QObject *parent)
    : QObject(parent)
{

}

QStandardItem* TreeView::createItem(Item it, const QString& s)
{
    QString resource;
    QString text;

    switch (it)
    {
    case TreeView::Item::Chart:
        resource = "v-chart";
        text = tr("Wykres");
        break;

    case TreeView::Item::AlternativeModelSerial:
        text = tr("Szeregowy");
        break;

    case TreeView::Item::AlternativeModelParallel:
        text = tr("Równoległy");
        break;

    case TreeView::Item::SetContainer:
        resource = "v-set";
        text = tr("Zbiory danych");

        break;

    case TreeView::Item::AlternativeModelContainer:
        resource = "v-rlc";
        text = tr("Modele zastępcze");
        break;
    }

    resource = ":/icons/"+resource+".png";

    text = s.isEmpty() ? text : s;

    QStandardItem* item = new QStandardItem(QIcon(resource), text);
    item->data(ItemRole) = QVariant(static_cast<int>(it));
    return item;
}
