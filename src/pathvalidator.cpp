#include "pathvalidator.h"

#include <fstream>
#include <QDebug>
#include <QDir>

PathValidator::PathValidator(QObject* parent) :
    QValidator(parent)
{ }

QValidator::State PathValidator::validate(QString &text, int &pos) const
{
    auto dir_path = QDir(text);

    std::ofstream file(dir_path.absolutePath().toStdString(), std::ios_base::ate);
    if (file)
    {
        file.close();
        if (!dir_path.exists()){
            qDebug() << "File removed? : " << dir_path.remove("");
        }
        return QValidator::Acceptable;
    }
    else
        return QValidator::Invalid;
}


