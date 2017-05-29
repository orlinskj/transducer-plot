#include "exportfilehandler.h"

#include "../viewmodel/transduceritem.h"
#include <boost/filesystem.hpp>
#include <QDebug>

ExportFileHandler::ExportFileHandler(TransducerItem* transducer, const std::string& ext) :
    transducer_(transducer),
    ext_(std::string(".")+ext)
{ }

std::string ExportFileHandler::path(const std::string& filepath)
{
    boost::filesystem::path path(filepath);

    if (path.extension().string() != ext_){
        path += ext_;
    }

    return path.string();
}

