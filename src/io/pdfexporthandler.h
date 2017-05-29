#ifndef PDFEXPORTHANDLER_H
#define PDFEXPORTHANDLER_H

#include "exportfilehandler.h"
#include <map>

class BVDSolver;
class TransducerItem;

class PDFExportHandler : public ExportFileHandler
{
public:
    struct Options{
        bool model_data;
        bool tab_data;
        bool plots;
    };

    PDFExportHandler(TransducerItem* transducer, const Options& options, BVDSolver* solver);
    int save(const std::string &filepath) override;

private:
    void add_hook(const std::string& hook, const std::string& val);
    void add_hook(const std::string& hook, double val);
    std::string replace_words(const std::string& str) const;

    Options options_;
    std::map<std::string, std::string> dict_;
    BVDSolver* solver_;
};

#endif // PDFEXPORTHANDLER_H
