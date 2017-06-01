#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <QToolButton>
#include <QWidget>
#include <QObject>

/**
 * @brief The PathFinder class
 * @desc Opens file search dialog and stores selected path in QLineEdit or QComboBox.
 */
class PathFinder : public QObject
{
public:
    static void show(QWidget* widget, QString filter = "");
};

#endif // PATHFINDER_H
