#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <QToolButton>
#include <QWidget>

class PathFinder : public QToolButton
{
public:
    PathFinder(QLineEdit* line, bool combo = false);
    void mousePressEvent(QMouseEvent* event) override;

private:
    QWidget* line_;
    bool combo_;
};

#endif // PATHFINDER_H
