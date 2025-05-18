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

// Infantry
Infantry::Infantry(int quantity, int weight, const Position pos, InfantryType infantryType)
    : Unit(quantity, weight, pos), infantryType(infantryType) {}
Infantry::~Infantry() {}
// Kiểm tra số chính phương
bool Infantry::isPerfectSquare(int n) const {
    if (n < 0) return false;
    int root = static_cast<int>(sqrt(n));
    return root * root == n;
}
// Kiểm tra số cá nhân
int Infantry::digitSum(int num) const {
    int sum = 0;
    while (num > 0) {
        sum += num % 10;
        num /= 10;
    }
    return sum;
}
int Infantry::personalNumber(int num, int year) const {
    int total = num + digitSum(year);
    while (total >= 10) {
        total = digitSum(total);
    }
    return total;
}
int Infantry::getAttackScore() {
    int typeValue = static_cast<int>(infantryType);
    int initialScore = typeValue * 56 + quantity * weight;
    int extraScore = 0;
    if (infantryType == SPECIALFORCES && isPerfectSquare(weight)) {
        extraScore = 75;
        initialScore += extraScore;
    }
    int pNumber = personalNumber(initialScore, 1975);
    if (pNumber > 7) {
        quantity = static_cast<int>(quantity * 1.2);
    } else if (pNumber < 3) {
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

// Army
Army::Army(Unit **unitArray, int size, string name, BattleField *battleField)
    : name(name), battleField(battleField), LF(0), EXP(0)
{
    unitList = new UnitList();
    for (int i = 0; i < size; i++)
    {
        const Unit *unit = unitArray[i];
        unitList->addUnit(unit);

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
Army::~Army() {}
int Army::getLF() { return LF; }
int Army::getExp() { return EXP; }
void Army::setLF(int lf) { LF = lf; }
void Army::setExp(int exp) { EXP = exp; }
Army::~Army() {}

// LiberationArmy
LiberationArmy::LiberationArmy(const Unit **unitArray, int size, string name, BattleField *battleField) : Army(unitArray, size, name, battleField) {}
string LiberationArmy::str() const
{
    return "LiberationArmy: " + name + " (LF=" + to_string(LF) + ", EXP=" + to_string(EXP) + ")";
}
LiberationArmy::~LiberationArmy() {}

void LiberationArmy::removeUnits(Unit unit)
{
}
void LiberationArmy::recalcIndices()
{
}
void LiberationArmy::confiscateEnemyUnits(Army *enemy)
{
}
vector<Unit *> LiberationArmy::findSmallestVehicleCombGreaterThan(int enemyLF)
{
}
vector<Unit *> LiberationArmy::findSmallestInfantryCombGreaterThan(int enemyEXP)
{
}
void LiberationArmy::eliminateAllInfantry()
{
}
void LiberationArmy::eliminateAllVehicles()
{
}
void LiberationArmy::reduceAllUnitsWeight(int num)
{
}
void LiberationArmy::reinforceUnitsWithFibonacci()
{
    // for (auto unit : unitList->getAllUnits()) {
    //     int q = unit->getQuantity();
    //     unit->setQuantity(nextFibonacci(q));
    // }
}
LiberationArmy::fight(Army *enemy, bool defense = false)
{
    if (defense)
    {
        float libreationLF = LF * 1.5;
        float libreationEXP = EXP * 1.5;
        float enemyLF = enemy->getLF();
        float enemyEXP = enemy->getEXP();
        // Victory
        if (libreationLF >= enemyLF && libreationEXP >= enemyEXP)
        {
            confiscateEnemyUnits(enemy);
            recalcIndices();
            return;
        }
        // Partial loss – 10% reduction
        if ((libreationLF < enemyLF && libreationEXP >= enemyEXP) ||
            (libreationLF >= enemyLF && libreationEXP < enemyEXP))
        {
            reduceAllUnitsWeight(10);
            recalcIndices();
            return;
        }
        if (libreationLF < enemyLF && libreationEXP < enemyEXP)
        {
            reinforceUnitsWithFibonacci();
            recalcIndices();

            // Recalculate indices
            libreationLF = getLF() * 1.3;
            libreationEXP = getEXP() * 1.3;

            if (libreationLF >= enemyLF && libreationEXP >= enemyEXP)
            {
                confiscateEnemyUnits(enemy);
                recalcIndices();
            }
            else if ((libreationLF < enemyLF && libreationEXP >= enemyEXP) ||
                     (libreationLF >= enemyLF && libreationEXP < enemyEXP))
            {
                reduceAllUnitsWeight(10);
                recalcIndices();
            }
        }
    }
    else
    {
        float libreationLF = LF * 1.5;
        float libreationEXP = EXP * 1.5;
        float enemyLF = enemy->getLF();
        float enemyEXP = enemy->getEXP();

        auto combInfantry = findSmallestInfantryCombGreaterThan(enemyEXP);
        auto combVehicle = findSmallestVehicleCombGreaterThan(enemyLF);

        bool hasCombInfantry = !combInfantry.empty();
        bool hasCombVehicle = !combVehicle.empty();

        // Case 1:  Tìm ra tổ hợp thoả mãn
        if (hasCombInfantry && hasCombVehicle)
        {
            // Xoá danh sách tổ hợp thoả mãn
            removeUnits(combInfantry);
            removeUnits(combVehicle);
            confiscateEnemyUnits(enemy);
            recalcIndices();
        }
        // Case 2:  Tìm ra 1 tổ hợp thoả mãn
        else if (hasCombInfantry || hasCombVehicle)
        {
            if (hasCombInfantry)
            {
                if (libreationLF > enemyLF)
                {
                    removeUnits(combInfantry);
                    eliminateAllVehicles();
                    confiscateEnemyUnits(enemy);
                    recalcIndices();
                }
                else
                {
                    // No battle
                    reduceAllUnitsWeight(10);
                    recalcIndices();
                }
            }
            else
            {
                if (libreationEXP > enemyEXP)
                {
                    removeUnits(combVehicle);
                    eliminateAllInfantry();
                    confiscateEnemyUnits(enemy);
                    recalcIndices();
                }
                else
                {
                    // No battle
                    reduceAllUnitsWeight(10);
                    recalcIndices();
                }
            }
        }
        else
        {
            reduceAllUnitsWeight(10);
            recalcIndices();
        }
    }
}

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////