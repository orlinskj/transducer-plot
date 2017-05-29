#ifndef PDFPRINTER_H
#define PDFPRINTER_H

#include "viewmodel/plotitem.h"
#include "model/transducer.h"
#include <vector>
#include <map>
#include <string>

struct PDFOptions
{
    // bool models;
    std::vector<PlotItem*> plots;
};

class PDFPrinter
{
public:
    void init(Transducer* transducer);
    int print(Transducer *transducer, const std::string& path, PDFOptions options);
    void add_hook(const std::string& hook, const std::string& val);
    std::string replace_words(const std::string& str) const;
protected:
    std::map<std::string, std::string> dict_;
};

#endif // PDFPRINTER_H
