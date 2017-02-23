#include "unitaxis.h"

UnitAxis::UnitAxis(const CombinedUnit& unit) :
    unit(unit)
{
    update();
}

UnitAxis::UnitAxis(const UnitAxis& axis) :
    unit(axis.unit)
{
    update();
}

std::string UnitAxis::name() const
{
    return unit.longname();
}

void UnitAxis::update()
{ }

void UnitAxis::add_unit(const Unit &u)
{
    unit.add_unit(u);
    update();
}

void UnitAxis::remove_unit(const Unit &u)
{
    unit.remove_unit(u);
    update();
}

bool UnitAxis::match_unit(const Unit &u)
{
    if (unit.unit() == u.unit())
        return true;
    return false;
}

bool UnitAxis::no_units() const
{
    return unit.no_units();
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
    this->setTitleText(UnitAxis::name().c_str());
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
    this->setTitleText(UnitAxis::name().c_str());
}

UnitAxis* UnitAxis::from_qabstractaxis(QAbstractAxis* axis)
{
    UnitAxis* unit_axis = dynamic_cast<UnitValueAxis*>(axis);
    if (!unit_axis)
        unit_axis = dynamic_cast<UnitLogValueAxis*>(axis);
    return unit_axis;
}
