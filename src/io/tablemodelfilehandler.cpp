#include "tablemodelfilehandler.h"
#include <QAbstractItemModel>
#include <fstream>

TableModelFileHandler::TableModelFileHandler(QAbstractItemModel* table) :
    model_(table)
{ }

int TableModelFileHandler::save(const std::string &path, const std::string &ext) const
{
    std::ofstream ostr(path);

    int count = model_->rowCount() * model_->columnCount();
    int colc = model_->columnCount();

    std::string row_sep = "\n";
    std::string col_sep = " ";

    // csv
    for (int i=0; i<count; ++i)
    {
        auto index = model_->index(i/colc,i%colc);
        ostr << model_->data(index).toString().toStdString();
        if ((i+1) % colc != 0)
            ostr << col_sep;
        else
            ostr << row_sep;
    }

    ostr.close();
}
