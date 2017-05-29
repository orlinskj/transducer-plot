#include "pathfinder.h"

#include <QFileDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QDebug>

void PathFinder::show(QWidget *widget, QString filter)
{
    auto lineedit = dynamic_cast<QLineEdit*>(widget);
    auto combobox = dynamic_cast<QComboBox*>(widget);

    QString def_path;

    if (lineedit)
        def_path = lineedit->text();
    else if(combobox)
        def_path = combobox->currentText();

    qDebug() << QFileInfo(def_path).absoluteFilePath();

    QString path = QFileDialog::getSaveFileName(
                widget,
                tr("Wskaż plik"),
                QFileInfo(def_path).absoluteFilePath(),
                filter);

    if (lineedit){
        if (!path.isEmpty())
            lineedit->setText(path);
    }
    else if(combobox){
        if (!path.isEmpty()) {
            if (combobox->findText(path) == -1){
                combobox->addItem(path);
            }
            combobox->setCurrentIndex(combobox->findText(path));
        }
    }
}
