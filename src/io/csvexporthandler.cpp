#include "csvexporthandler.h"

#include "../viewmodel/transducertableproxymodel.h"

CSVExportHandler::CSVExportHandler(
        TransducerItem* transducer,
        const CSVExportHandler::Options& options,
        QAbstractProxyModel* table_model) :
    ExportFileHandler(transducer, "csv"), options_(options), model_(table_model)
{

}

int CSVExportHandler::save(const std::string &filepath)
{
    std::ofstream ostr(this->path(filepath));
    if (!ostr)
        return -1;

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
    return 0;
}
