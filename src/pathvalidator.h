#ifndef PATHVALIDATOR_H
#define PATHVALIDATOR_H

#include <QValidator>

class PathValidator : public QValidator
{
public:
    PathValidator(QObject* parent = nullptr);
    QValidator::State validate(QString &text, int &pos) const override;
};

#endif // PATHVALIDATOR_H
