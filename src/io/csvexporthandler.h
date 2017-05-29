#ifndef CSVEXPORTHANDLER_H
#define CSVEXPORTHANDLER_H

#include "exportfilehandler.h"

#include <QAbstractProxyModel>

class CSVExportHandler : public ExportFileHandler
{
public:
    struct Options{
        bool model_params;
        bool header;
        bool tab_data;
    };

    CSVExportHandler(TransducerItem* transducer, const Options& options, QAbstractProxyModel* table_model);
    int save(const std::string &filepath) override;

private:
    Options options_;
    QAbstractProxyModel* model_;
};

#endif // CSVEXPORTHANDLER_H
