#ifndef SIZEVALIDATOR_H
#define SIZEVALIDATOR_H

#include <QRegExpValidator>

class SizeValidator : public QRegExpValidator
{
public:
    SizeValidator(QObject* parent = nullptr);
    void fixup(QString &s) const;
};

#endif // SIZEVALIDATOR_H
