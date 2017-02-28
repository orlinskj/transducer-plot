#ifndef TRANSDUCERTABLEDATAFILEHANDLER_H
#define TRANSDUCERTABLEDATAFILEHANDLER_H

#include <string>

class QAbstractItemModel;

class TableModelFileHandler
{
public:
    TableModelFileHandler(QAbstractItemModel* table_model);
    int save(const std::string& path, const std::string& ext = "") const;

protected:
    QAbstractItemModel* model_;
};

#endif // TRANSDUCERTABLEDATAFILEHANDLER_H
