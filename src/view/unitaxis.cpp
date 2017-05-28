#include "unitaxis.h"

UnitAxis::UnitAxis(const CombinedUnit& unit) :
    unit_(unit)
{
    update();
}

UnitAxis::UnitAxis(const UnitAxis& axis) :
    unit_(axis.unit_)
{
    update();
}

void UnitAxis::update()
{
}

CombinedUnit& UnitAxis::unit()
{
    return unit_;
}

UnitAxis::Type UnitAxis::unit_axis_type() const
{
    return UnitAxis::Type::None;
}


UnitValueAxis::UnitValueAxis(const CombinedUnit &unit) :
    UnitAxis(unit)
{
    update();
}

UnitValueAxis::UnitValueAxis(const UnitAxis &axis) :
    UnitAxis(axis)
{
    update();
}

void UnitValueAxis::update()
{
    this->setTitleText(unit().longname().c_str());
}

UnitAxis::Type UnitValueAxis::unit_axis_type() const
{
    return UnitAxis::Type::Value;
}

UnitLogValueAxis::UnitLogValueAxis(const CombinedUnit &unit) :
    UnitAxis(unit)
{
    update();
}

UnitLogValueAxis::UnitLogValueAxis(const UnitAxis &axis) :
    UnitAxis(axis)
{
    update();
}

void UnitLogValueAxis::update()
{
    this->setTitleText(unit().longname().c_str());
}

UnitAxis::Type UnitLogValueAxis::unit_axis_type() const
{
    return UnitAxis::Type::LogValue;
}

UnitAxis* UnitAxis::from_qabstractaxis(QAbstractAxis* axis)
{
    UnitAxis* unit_axis = dynamic_cast<UnitValueAxis*>(axis);
    if (!unit_axis)
        unit_axis = dynamic_cast<UnitLogValueAxis*>(axis);
    return unit_axis;
}
