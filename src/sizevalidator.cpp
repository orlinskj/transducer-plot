#include "sizevalidator.h"

SizeValidator::SizeValidator(QObject* parent) :
    QRegExpValidator(parent)
{
    auto re = "([1-9]\\d{2,3})\\D([1-9]\\d{2,3})";
    this->setRegExp(QRegExp(re));
}

void SizeValidator::fixup(QString &s) const
{
    if (s.isEmpty())
        s = "800x600";
    else
        s.append(QString("x")+s);
}
