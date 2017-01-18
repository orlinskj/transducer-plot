#ifndef FUNCTIONDIALOG_H
#define FUNCTIONDIALOG_H

#include <QDialog>

namespace Ui {
class FunctionDialog;
}

class FunctionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FunctionDialog(QWidget *parent = 0);
    ~FunctionDialog();

    void setTransducerModel();

private:
    Ui::FunctionDialog *ui;
};

#endif // FUNCTIONDIALOG_H
