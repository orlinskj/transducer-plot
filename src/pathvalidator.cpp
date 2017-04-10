#include "pathvalidator.h"

#include <fstream>
#include <boost/filesystem.hpp>
#include <QDebug>

PathValidator::PathValidator(QObject* parent) :
    QValidator(parent)
{ }

QValidator::State PathValidator::validate(QString &text, int &pos) const
{
    auto path = boost::filesystem::absolute(text.toStdString());
    bool exists = boost::filesystem::exists(path);

    std::ofstream file(path.string(),std::ios_base::ate);
    if (file)
    {
        file.close();
        if (!exists)
            boost::filesystem::remove(path);
        return QValidator::Acceptable;
    }
    else
        return QValidator::Invalid;
}


