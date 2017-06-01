#include "screenshotform.h"
#include "ui_screenshotform.h"

#include "view/plotpresenter.h"
#include "pathfinder.h"
#include "sizevalidator.h"
#include <regex>
#include <QComboBox>
#include <QMessageBox>

ScreenshotForm::ScreenshotForm(QWidget *parent, PlotPresenter* presenter) :
    QWidget(parent),
    ui(new Ui::ScreenshotForm),
    presenter_(presenter)
{
    ui->setupUi(this);
    this->setVisible(false);
    this->setPalette(parent->palette());
    this->move(6,6);

    // sizes setup
    QList<QStandardItem*> items = {
        new QStandardItem("640x480"),
        new QStandardItem("800x600"),
        new QStandardItem("1024x768"),
        new QStandardItem("1600x1200")
    };
    sizes_.appendColumn(items);
    ui->comboBoxSize->setModel(&sizes_);
    size_validator_ = new SizeValidator(ui->comboBoxSize);
    ui->comboBoxSize->setValidator(size_validator_);

    connect(ui->buttonClose, &QToolButton::clicked,
            this, [this](){ this->hide(); });
    connect(ui->buttonSave, &QToolButton::clicked,
            this, &ScreenshotForm::save);

    connect(ui->comboBoxSize, &QComboBox::currentTextChanged,
            this, &ScreenshotForm::enable_save_button);

    connect(ui->pathPushButton, &QPushButton::clicked,
            this, [this](){ PathFinder::show(ui->pathLineEdit, tr("Obrazy (*.png)")); });
}

ScreenshotForm::~ScreenshotForm()
{
    delete ui;
}

void ScreenshotForm::save()
{
    auto re = size_validator_->regExp();
    re.indexIn(ui->comboBoxSize->currentText());
    int width = re.cap(1).toInt();
    int height = re.cap(2).toInt();

    QImage img = presenter_->screenshot(width,height);
    QString path = QDir(ui->pathLineEdit->text()).absolutePath();

    if (QDir().exists(path)){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(
                    this,
                    tr("Zastąp plik"),
                    tr("Plik o podanej ścieżce")+" "+path+" "+tr("istnieje. Nadpisać plik?"),
                    QMessageBox::Yes|QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            img.save(ui->pathLineEdit->text(),nullptr,-1);
            this->hide();
        }
    }
    else {
        img.save(ui->pathLineEdit->text(),nullptr,-1);
        this->hide();
    }
}

void ScreenshotForm::enable_save_button(const QString&)
{
    std::regex re("([1-9]\\d{1,3})\\D+([1-9]\\d{1,3})");
    std::smatch match;
    std::string target = ui->comboBoxSize->currentText().toStdString();
    if (std::regex_match(target,match,re))
    {
        ui->buttonSave->setEnabled(true);
    }
    else
        ui->buttonSave->setEnabled(false);
}
