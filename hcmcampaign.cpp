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
Vehicle::Vehicle(VehicleType vehicleType, int quantity, int weight, const Position pos)
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

// Infantry
Infantry::Infantry(InfantryType infantryType, int quantity, int weight, const Position pos)
    : Unit(quantity, weight, pos), infantryType(infantryType) {}

Infantry::~Infantry() {}

// Kiểm tra số chính phương
bool Infantry::isPerfectSquare(int n) const
{
    if (n < 0)
        return false;
    int root = static_cast<int>(sqrt(n));
    return root * root == n;
}
// Kiểm tra số cá nhân
int Infantry::digitSum(int num) const
{
    int sum = 0;
    while (num > 0)
    {
        sum += num % 10;
        num /= 10;
    }
    return sum;
}
int Infantry::personalNumber(int num, int year) const
{
    int total = num + digitSum(year);
    if (total >= 10)
    {
        return digitSum(total, 0);
    }
    return total;
}
int Infantry::getAttackScore()
{
    int typeValue = static_cast<int>(infantryType);
    int initialScore = typeValue * 56 + quantity * weight;
    int extraScore = 0;
    if (infantryType == SPECIALFORCES && isPerfectSquare(weight))
    {
        extraScore = 75;
        initialScore += extraScore;
    }
    int pNumber = personalNumber(initialScore);
    if (pNumber > 7)
    {
        quantity = static_cast<int>(quantity * 1.2);
    }
    else if (pNumber < 3)
    {
        quantity = static_cast<int>(quantity * 0.9);
    }
    int finalScore = typeValue * 56 + quantity * weight + extraScore;
    return finalScore;
}
string Infantry::getStringType() const
{
    switch (infantryType)
    {
    case SNIPER:
        return "SNIPER";
    case ANTIAIRCRAFTSQUAD:
        return "ANTIAIRCRAFTSQUAD";
    case MORTARSQUAD:
        return "MORTARSQUAD";
    case ENGINEER:
        return "ENGINEER";
    case SPECIALFORCES:
        return "SPECIALFORCES";
    case REGULARINFANTRY:
        return "REGULARINFANTRY";
    default:
        return "UNKNOWN";
    }
}
string Infantry::str() const
{
    stringstream ss;
    ss << "Infantry[infantryType=" << getStringType()
       << ",quantity=" << quantity
       << ",weight=" << weight
       << ",pos=" << pos.str()
       << "]";
    return ss.str();
}

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////