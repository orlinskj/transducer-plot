#ifndef EXPORTFILEHANDLER_H
#define EXPORTFILEHANDLER_H

#include <fstream>

class TransducerItem;

class ExportFileHandler
{
public:
    ExportFileHandler(TransducerItem* transducer, const std::string& ext);
    virtual int save(const std::string& path) = 0;

protected:
    std::string path(const std::string& filepath);

    TransducerItem* transducer_;
    std::string ext_;
};

#endif // EXPORTFILEHANDLER_H
