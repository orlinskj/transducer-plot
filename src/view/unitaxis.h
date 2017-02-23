#ifndef UNITAXIS_H
#define UNITAXIS_H

#include "../model/unit.h"
#include <QValueAxis>
#include <QLogValueAxis>

using namespace QtCharts;

class UnitAxis;
class UnitValueAxis;
class UnitLogValueAxis;

// NOTE: That is not the best design, because QValueAxis and QLogValueAxis
// does not derive virtually from QbstractAxis...

class UnitAxis {
public:
    UnitAxis() = default;
    UnitAxis(const CombinedUnit& unit);
    UnitAxis(const UnitAxis& axis);

    void add_unit(const Unit& u);
    void remove_unit(const Unit& u);
    bool match_unit(const Unit& u);
    bool no_units() const;
    std::string name() const;
    virtual void update();

    static UnitAxis* from_qabstractaxis(QAbstractAxis* axis);

private:
    CombinedUnit unit;
};

class UnitValueAxis : public QValueAxis, public UnitAxis {
public:
    UnitValueAxis() = default;
    UnitValueAxis(const CombinedUnit& unit);
    UnitValueAxis(const UnitAxis& axis);
    void update() override;
};

class UnitLogValueAxis : public QLogValueAxis, public UnitAxis {
public:
    UnitLogValueAxis() = default;
    UnitLogValueAxis(const CombinedUnit& unit);
    UnitLogValueAxis(const UnitAxis& axis);
    void update() override;
};

#endif // UNITAXIS_H
