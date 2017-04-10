#ifndef SCREENSHOTFORM_H
#define SCREENSHOTFORM_H

#include <QWidget>
#include <QStandardItemModel>
#include <QRegExpValidator>

namespace Ui {
class ScreenshotForm;
}

class PlotPresenter;
class SizeValidator;

class ScreenshotForm : public QWidget
{
    Q_OBJECT

public:
    explicit ScreenshotForm(QWidget *parent = nullptr, PlotPresenter* presenter = nullptr);
    ~ScreenshotForm();

public slots:
    void save();
    void enable_save_button(const QString&);

private:
    Ui::ScreenshotForm *ui;
    QStandardItemModel sizes_;
    PlotPresenter* presenter_;

    SizeValidator* size_validator_;
};

#endif // SCREENSHOTFORM_H
