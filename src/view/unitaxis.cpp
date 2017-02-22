#include "unitaxis.h"

UnitAxis::UnitAxis(const CombinedUnit& unit) :
    unit(unit)
{ }

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

bool UnitAxis::unit_match(const Unit &u)
{
    /// TODO
    return false;
}

UnitValueAxis::UnitValueAxis(const CombinedUnit &unit) :
    UnitAxis(unit)
{ }

void UnitValueAxis::update()
{
    this->setTitleText(UnitAxis::name().c_str());
}

UnitLogValueAxis::UnitLogValueAxis(const CombinedUnit &unit) :
    UnitAxis(unit)
{ }

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
