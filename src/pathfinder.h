#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <QToolButton>
#include <QWidget>
#include <QObject>

class PathFinder : public QObject
{
public:
    static void show(QWidget* widget, QString filter = "");
};

#endif // PATHFINDER_H
