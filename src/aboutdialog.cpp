#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include "transducerplotversion.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    ui->versionLabel->setText(TRANSDUCER_PLOT_VERSION);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
