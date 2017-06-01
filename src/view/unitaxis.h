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
// does not derive virtually from QAbstractAxis

/**
 * @brief The UnitAxis class
 *
 * Represents axis with attached units. Helps with handling multiple units on one axis and displaying them nicely.
 */
class UnitAxis {
public:
    enum class Type {
        None,
        Value,
        LogValue
    };

    UnitAxis() = default;
    UnitAxis(const CombinedUnit& unit_);
    UnitAxis(const UnitAxis& axis);

    CombinedUnit& unit();
    virtual void update();
    virtual Type unit_axis_type() const;

    static UnitAxis* from_qabstractaxis(QAbstractAxis* axis);

private:
    CombinedUnit unit_;
};

class UnitValueAxis : public QValueAxis, public UnitAxis {
public:
    UnitValueAxis() = default;
    UnitValueAxis(const CombinedUnit& unit);
    UnitValueAxis(const UnitAxis& axis);
    void update() override;
    UnitAxis::Type unit_axis_type() const override;
};

class UnitLogValueAxis : public QLogValueAxis, public UnitAxis {
public:
    UnitLogValueAxis() = default;
    UnitLogValueAxis(const CombinedUnit& unit);
    UnitLogValueAxis(const UnitAxis& axis);
    void update() override;
    UnitAxis::Type unit_axis_type() const override;
};

#endif // UNITAXIS_H
