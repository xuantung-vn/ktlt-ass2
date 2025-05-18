#include "hcmcampaign.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
////////////////////////////////////////////////////////////////////////

// Helper
int nextFibonacci(int n)
{
    if (n <= 1)
        return 1;
    int a = 1, b = 1;
    while (b < n)
    {
        int temp = b;
        b = a + b;
        a = temp;
    }
    return b;
}
// Unit
Unit::Unit(int quantity, int weight, const Position pos)
    : quantity(quantity), weight(weight), pos(pos) {}
Unit::~Unit() {}
Unit::setQuantity(int quan)
{
    quantity = quan;
}
Unit::setWeight(int w)
{
    weight = w;
}
int Unit::getWeight() const { return weight; }
int Unit::getQuantity() const { return quantity; }
Unit::setPos(Position p)
{
    pos = p;
}
Position Unit::getCurrentPosition() const
{
    return pos;
}

// Vehicle
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
VehicleType Vehicle::getVehicleType() const { return vehicleType; }

// Infantry
Infantry::Infantry(int quantity, int weight, const Position pos, InfantryType infantryType)
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
    while (total >= 10)
    {
        total = digitSum(total);
    }
    return total;
}
int Infantry::getAttackScore() const
{
    int typeValue = static_cast<int>(infantryType);
    int initialScore = typeValue * 56 + quantity * weight;
    int extraScore = 0;
    if (infantryType == SPECIALFORCES && isPerfectSquare(weight))
    {
        extraScore = 75;
        initialScore += extraScore;
    }
    int pNumber = personalNumber(initialScore, 1975);
    int adjustedQuantity = quantity;
    if (pNumber > 7)
    {
        adjustedQuantity = static_cast<int>(quantity * 1.2);
    }
    else if (pNumber < 3)
    {
        adjustedQuantity = static_cast<int>(quantity * 0.9);
    }
    return typeValue * 56 + adjustedQuantity * weight + extraScore;
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

// Army
Army::Army(Unit **unitArray, int size, string name, BattleField *battleField)
    : name(name), battleField(battleField), LF(0), EXP(0)
{
    unitList = new UnitList();
    for (int i = 0; i < size; i++)
    {
        const Unit *unit = unitArray[i];
        unitList->insert(unit);

        const Vehicle *vehicle = dynamic_cast<const Vehicle *>(unit);
        if (vehicle)
        {
            LF += vehicle->getAttackScore();
            if (LF > 1000)
                LF = 1000;
            if (LF < 0)
                LF = 0;
            continue;
        }
        const Infantry *infantry = dynamic_cast<const Infantry *>(unit);
        if (infantry)
        {
            EXP += infantry->getAttackScore();
            if (EXP > 500)
                EXP = 500;
            if (EXP < 0)
                EXP = 0;
        }
    }
}
Army::~Army() { delete unitList; }
int Army::getLF() { return LF; }
int Army::getExp() { return EXP; }
void Army::setLF(int lf) { LF = lf; }
void Army::setExp(int exp) { EXP = exp; }

// LiberationArmy
LiberationArmy::LiberationArmy(const Unit **unitArray, int size, string name, BattleField *battleField) : Army(unitArray, size, name, battleField) {}
string LiberationArmy::str() const
{
    return "LiberationArmy: " + name + " (LF=" + to_string(LF) + ", EXP=" + to_string(EXP) + ")";
}
LiberationArmy::~LiberationArmy()
{ // unitList is deleted by base class Army
}

void LiberationArmy::removeUnits(const vector<Unit *> &units)
{
    for (Unit *unit : units)
    {
        unitList->removeUnit(unit);
    }
}
void LiberationArmy::recalcIndices()
{
    LF = 0;
    EXP = 0;
    for (Unit *unit : unitList->getAllUnits())
    {
        if (Vehicle *vehicle = dynamic_cast<Vehicle *>(unit))
        {
            LF += vehicle->getAttackScore();
            if (LF > 1000)
                LF = 1000;
            if (LF < 0)
                LF = 0;
        }
        else if (Infantry *infantry = dynamic_cast<Infantry *>(unit))
        {
            EXP += infantry->getAttackScore();
            if (EXP > 500)
                EXP = 500;
            if (EXP < 0)
                EXP = 0;
        }
    }
}
void LiberationArmy::confiscateEnemyUnits(Army *enemy)
{
    for (Unit *unit : enemy->unitList->getAllUnits())
    {
        unitList->insert(unit);
    }
    enemy->unitList->clear();
    enemy->setLF(0);
    enemy->setExp(0);
}
vector<Unit *> LiberationArmy::findSmallestInfantryCombGreaterThan(int enemyEXP)
{
    vector<Unit *> infantry;
    vector<Unit *> result;
    int minScore = INT_MAX;

    for (Unit *unit : unitList->getAllUnits())
    {
        if (dynamic_cast<Infantry *>(unit))
        {
            infantry.push_back(unit);
        }
    }

    int n = infantry.size();
    for (int mask = 1; mask < (1 << n); mask++)
    {
        int score = 0;
        vector<Unit *> temp;
        for (int i = 0; i < n; i++)
        {
            if (mask & (1 << i))
            {
                score += dynamic_cast<Infantry *>(infantry[i])->getAttackScore();
                temp.push_back(infantry[i]);
            }
        }
        if (score > enemyEXP && score < minScore)
        {
            minScore = score;
            result = temp;
        }
    }
    return result;
}
vector<Unit *> LiberationArmy::findSmallestVehicleCombGreaterThan(int enemyLF)
{
    vector<Unit *> vehicles;
    vector<Unit *> result;
    int minScore = INT_MAX;

    for (Unit *unit : unitList->getAllUnits())
    {
        if (dynamic_cast<Vehicle *>(unit))
        {
            vehicles.push_back(unit);
        }
    }

    int n = vehicles.size();
    for (int mask = 1; mask < (1 << n); mask++)
    {
        int score = 0;
        vector<Unit *> temp;
        for (int i = 0; i < n; i++)
        {
            if (mask & (1 << i))
            {
                score += dynamic_cast<Vehicle *>(vehicles[i])->getAttackScore();
                temp.push_back(vehicles[i]);
            }
        }
        if (score > enemyLF && score < minScore)
        {
            minScore = score;
            result = temp;
        }
    }
    return result;
}
void LiberationArmy::eliminateAllInfantry()
{
    vector<Unit *> toRemove;
    for (Unit *unit : unitList->getAllUnits())
    {
        if (dynamic_cast<Infantry *>(unit))
        {
            toRemove.push_back(unit);
        }
    }
    for (Unit *unit : toRemove)
    {
        unitList->removeUnit(unit);
    }
}
void LiberationArmy::eliminateAllVehicles()
{
    vector<Unit *> toRemove;
    for (Unit *unit : unitList->getAllUnits())
    {
        if (dynamic_cast<Vehicle *>(unit))
        {
            toRemove.push_back(unit);
        }
    }
    for (Unit *unit : toRemove)
    {
        unitList->removeUnit(unit);
    }
}
void LiberationArmy::reduceAllUnitsWeight(int percentage)
{
    for (Unit *unit : unitList->getAllUnits())
    {
        int currentWeight = unit->getWeight();
        int newWeight = currentWeight * (100 - percentage) / 100;
        unit->setWeight(newWeight);
    }
}

void LiberationArmy::reinforceUnitsWithFibonacci()
{
    for (Unit *unit : unitList->getAllUnits())
    {
        int q = unit->getQuantity();
        unit->setQuantity(nextFibonacci(q));
    }
}
void LiberationArmy::fight(Army *enemy, bool defense)
{
    if (!enemy)
        return; // Handle null enemy

    float liberationLF = LF * (defense ? 1.3 : 1.5);
    float liberationEXP = EXP * (defense ? 1.3 : 1.5);
    float enemyLF = enemy->getLF();
    float enemyEXP = enemy->getEXP();

    if (defense)
    {
        // Victory
        if (liberationLF >= enemyLF && liberationEXP >= enemyEXP)
        {
            confiscateEnemyUnits(enemy);
            recalcIndices();
            return;
        }
        // Partial loss or defeat
        if (liberationLF < enemyLF && liberationEXP < enemyEXP)
        {
            reinforceUnitsWithFibonacci();
            recalcIndices();
            liberationLF = getLF() * 1.3;
            liberationEXP = getEXP() * 1.3;
            if (liberationLF >= enemyLF && liberationEXP >= enemyEXP)
            {
                confiscateEnemyUnits(enemy);
            }
            else
            {
                reduceAllUnitsWeight(10);
            }
        }
        else
        {
            reduceAllUnitsWeight(10);
        }
        recalcIndices();
    }
    else
    {
        auto combInfantry = findSmallestInfantryCombGreaterThan(enemyEXP);
        auto combVehicle = findSmallestVehicleCombGreaterThan(enemyLF);

        bool hasCombInfantry = !combInfantry.empty();
        bool hasCombVehicle = !combVehicle.empty();

        if (hasCombInfantry && hasCombVehicle)
        {
            removeUnits(combInfantry);
            removeUnits(combVehicle);
            confiscateEnemyUnits(enemy);
        }
        else if (hasCombInfantry && liberationLF > enemyLF)
        {
            removeUnits(combInfantry);
            eliminateAllVehicles();
            confiscateEnemyUnits(enemy);
        }
        else if (hasCombVehicle && liberationEXP > enemyEXP)
        {
            removeUnits(combVehicle);
            eliminateAllInfantry();
            confiscateEnemyUnits(enemy);
        }
        else
        {
            reduceAllUnitsWeight(10);
        }
        recalcIndices();
    }
}
// Unit List
UnitList::UnitList(int capacity) : capacity(capacity), size(0)
{
    units = new Unit *[capacity];
}

UnitList::~UnitList()
{
    for (int i = 0; i < size; i++)
    {
        delete units[i]; // Assume UnitList owns the units
    }
    delete[] units;
}
bool UnitList::isContain(VehicleType vehicleType)
{
    for (int i = 0; i < size; i++)
    {
        if (Vehicle *vehicle = dynamic_cast<Vehicle *>(units[i]))
        {
            if (vehicle->getVehicleType() == vehicleType)
            {
                return true;
            }
        }
    }
    return false;
}
bool UnitList::isContain(InfantryType infantryType)
{
    for (int i = 0; i < size; i++)
    {
        if (Infantry *infantry = dynamic_cast<Infantry *>(units[i]))
        {
            if (infantry->getStringType() == infantryTypeToString(infantryType))
            {
                return true;
            }
        }
    }
    return false;
}
bool UnitList::insert(Unit *unit)
{
    if (size >= capacity)
        return false;
    units[size++] = unit;
    return true;
}
string UnitList::str() const
{
    stringstream ss;
    ss << "UnitList[";
    for (int i = 0; i < size; i++)
    {
        ss << units[i]->str();
        if (i < size - 1)
            ss << ",";
    }
    ss << "]";
    return ss.str();
}
vector<Unit *> UnitList::getAllUnits() const
{
    vector<Unit *> result;
    for (int i = 0; i < size; i++)
    {
        result.push_back(units[i]);
    }
    return result;
}

void UnitList::removeUnit(Unit *unit)
{
    for (int i = 0; i < size; i++)
    {
        if (units[i] == unit)
        {
            for (int j = i; j < size - 1; j++)
            {
                units[j] = units[j + 1];
            }
            size--;
            break;
        }
    }
}
void UnitList::clear()
{
    for (int i = 0; i < size; i++)
    {
        delete units[i];
    }
    size = 0;
}
void UnitList::clear()
{
    for (int i = 0; i < size; i++)
    {
        delete units[i];
    }
    size = 0;
}
string UnitList::vehicleTypeToString(VehicleType type) const
{
    switch (type)
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

string UnitList::infantryTypeToString(InfantryType type) const
{
    switch (type)
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
////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////