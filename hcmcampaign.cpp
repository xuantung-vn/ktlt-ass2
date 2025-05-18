#include "hcmcampaign.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
////////////////////////////////////////////////////////////////////////

// Unit
Unit::Unit(int quantity, int weight, const Position pos)
    : quantity(quantity), weight(weight), pos(pos) {}
Unit::~Unit() {}

Position Unit::getCurrentPosition() const
{
    return pos;
}

// Vehicel
Vehicle::Vehicle(VehicleType type, int quantity, int weight, const Position pos)
    : Unit(quantity, weight, pos), vehicleType(vehicleType) {}

Vehicle::~Vehicle() {}

int Vehicle::getAttackScore()
{
    return static_cast<int>(vehicleType) * 304 + (quantity * weight) / 30;
}
string Vehicle::getStringType() const
{
    switch (vehicleType)
    {
    case TRUCK:
        return "TRUCK";
    case MORTAR:
        return "MORTAR";
    case ANTIAIRCRAFT:
        return "ANTIAIRCRAFT";
    case ARMOREDCAR:
        return "ARMOREDCAR";
    case APC:
        return "APC";
    case ARTILLERY:
        return "ARTILLERY";
    case TANK:
        return "TANK";
    default:
        return "UNKNOWN";
    }
}
string Vehicle::str() const
{
    stringstream ss;
    ss << "Vehicle[vehicleType=" << getStringType()
       << ",quantity=" << quantity
       << ",weight=" << weight
       << ",pos=" << pos.str()
       << "]";
    return ss.str();
}
////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////