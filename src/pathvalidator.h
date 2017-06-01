#ifndef PATHVALIDATOR_H
#define PATHVALIDATOR_H

#include <QValidator>

/// NOTE: currently not used in code

/**
 * @brief The PathValidator class
 * @desc Checks whether directory is writeable.
 */
class PathValidator : public QValidator
{
public:
    PathValidator(QObject* parent = nullptr);
    QValidator::State validate(QString &text, int &pos) const override;
};

#endif // PATHVALIDATOR_H
