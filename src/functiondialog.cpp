#include "functiondialog.h"
#include "ui_functiondialog.h"

FunctionDialog::FunctionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FunctionDialog)
{
    ui->setupUi(this);
}

FunctionDialog::~FunctionDialog()
{
    delete ui;
}
