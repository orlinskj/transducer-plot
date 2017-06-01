#include "exportfilehandler.h"

#include "../viewmodel/transduceritem.h"
#include <QDebug>
#include <experimental/filesystem>

ExportFileHandler::ExportFileHandler(TransducerItem* transducer, const std::string& ext) :
    transducer_(transducer),
    ext_(std::string(".")+ext)
{ }

std::string ExportFileHandler::path(const std::string& filepath)
{
    using path = std::experimental::filesystem::path;
    path fpath(filepath);

    if (fpath.extension() != ext_){
        fpath += ext_;
    }

    return fpath;
}

