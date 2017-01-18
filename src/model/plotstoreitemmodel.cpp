#include "plotstoreitemmodel.h"

#include <memory>
#include <QModelIndex>

ac::PlotStoreItemModel::PlotStoreItemModel(QObject* parent) :
    TreeItemModel(parent){ }

ac::PlotStoreItemModel::~PlotStoreItemModel() { }
