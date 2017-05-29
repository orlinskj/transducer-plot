#include "pathfinder.h"

#include <QFileDialog>
#include <QComboBox>

PathFinder::PathFinder(QLineEdit *line, bool combo) : line_(line), combo_(combo)
{

}

void PathFinder::mousePressEvent(QMouseEvent *event)
{
    QString path = QFileDialog::getSaveFileName(this,
                                tr("Find Files"), QDir::currentPath());

    if (combo_){
        if (!path.isEmpty()) {
            auto combobox = dynamic_cast<QComboBox*>(widget_);
            if (directoryComboBox->findText(directory) == -1)
                directoryComboBox->addItem(directory);
            directoryComboBox->setCurrentIndex(directoryComboBox->findText(directory));
        }
    }
    line_->setText(path);
    QToolButton::mousePressEvent(event);
}
