#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <qt5/QtGui/QStandardItem>
#include <qt5/QtCore/QObject>

namespace ac {

class TreeView : public QObject
{

public:
    TreeView(QObject* parent);

    static constexpr int ItemRole =     Qt::UserRole+1024;
    static constexpr int PointerRole =  Qt::UserRole+1025;

    enum class Item {
        Chart,
        DataSet,
        SetContainer,
        Set,
        AlternativeModelContainer,
        AlternativeModel,
        AlternativeModelSerial,
        AlternativeModelParallel
    };

    static QStandardItem* createItem(Item it, const QString &s = QString() );
};

}

#endif // TREEVIEW_H
