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
bool isSpecialNumber(int n, int k)
{
    if (n < 0)
        return false;
    vector<int> powers;
    int power = 1;
    while (power <= n)
    {
        powers.push_back(power);
        if (power > n / k)
            break;
        power *= k;
    }
    int m = powers.size();
    for (int mask = 0; mask < (1 << m); mask++)
    {
        int sum = 0;
        for (int i = 0; i < m; i++)
        {
            if (mask & (1 << i))
            {
                sum += powers[i];
            }
        }
        if (sum == n)
            return true;
    }
    return false;
}
double euclideanDistance(const Position &p1, const Position &p2)
{
    int dr = p1.getRow() - p2.getRow();
    int dc = p1.getCol() - p2.getCol();
    return std::sqrt(dr * dr + dc * dc);
}
Unit *parseUnit(const string &unitStr)
{
    string cleaned = trim(unitStr);
    size_t openParen = cleaned.find('(');
    if (openParen == string::npos)
        return nullptr;

    string unitName = trim(cleaned.substr(0, openParen));
    string params = cleaned.substr(openParen + 1, cleaned.find_last_of(')') - openParen - 1);
    stringstream ss(params);
    string quantityStr, weightStr, posStr, armyStr;
    if (!getline(ss, quantityStr, ',') || !getline(ss, weightStr, ',') ||
        !getline(ss, posStr, ',') || !getline(ss, armyStr, ','))
    {
        return nullptr;
    }

    try
    {
        int quantity = stoi(trim(quantityStr));
        int weight = stoi(trim(weightStr));
        int armyBelong = stoi(trim(armyStr));
        if (quantity <= 0 || weight <= 0 || (armyBelong != 0 && armyBelong != 1))
        {
            return nullptr;
        }
        Position *pos = parsePosition(posStr);
        if (!pos)
            return nullptr;

        VehicleType vType;
        InfantryType iType;
        bool isVehicle;
        if (!getUnitType(unitName, vType, iType, isVehicle))
        {
            delete pos;
            return nullptr;
        }

        Unit *unit = nullptr;
        if (isVehicle)
        {
            unit = new Vehicle(vType, quantity, weight, *pos);
        }
        else
        {
            unit = new Infantry(iType, quantity, weight, *pos);
        }
        delete pos;
        return unit;
    }
    catch (...)
    {
        return nullptr;
    }
}
string trim(const string &s)
{
    string result = s;
    result.erase(0, result.find_first_not_of(" \t"));
    result.erase(result.find_last_not_of(" \t") + 1);
    return result;
}

Position *parsePosition(const string &posStr)
{
    string cleaned = trim(posStr);
    if (cleaned.size() < 3 || cleaned[0] != '(' || cleaned[cleaned.size() - 1] != ')')
    {
        return nullptr;
    }
    cleaned = cleaned.substr(1, cleaned.size() - 2); // Remove ( )
    size_t comma = cleaned.find(',');
    if (comma == string::npos)
        return nullptr;
    try
    {
        int r = stoi(cleaned.substr(0, comma));
        int c = stoi(cleaned.substr(comma + 1));
        return new Position(r, c);
    }
    catch (...)
    {
        return nullptr;
    }
}

vector<Position *> parseTerrainArray(const string &value)
{
    vector<Position *> positions;
    if (value.size() < 2 || value[0] != '[' || value[value.size() - 1] != ']')
    {
        return positions;
    }
    string inner = trim(value.substr(1, value.size() - 2));
    if (inner.empty())
        return positions;

    stringstream ss(inner);
    string posStr;
    while (getline(ss, posStr, ','))
    {
        Position *pos = parsePosition(posStr);
        if (pos)
            positions.push_back(pos);
    }
    return positions;
}

bool getUnitType(const string &name, VehicleType &vType, InfantryType &iType, bool &isVehicle)
{
    static const map<string, VehicleType> vehicleMap = {
        {"TRUCK", TRUCK}, {"MORTAR", MORTAR}, {"ANTIAIRCRAFT", ANTIAIRCRAFT}, {"ARMOREDCAR", ARMOREDCAR}, {"APC", APC}, {"ARTILLERY", ARTILLERY}, {"TANK", TANK}};
    static const map<string, InfantryType> infantryMap = {
        {"SNIPER", SNIPER}, {"ANTIAIRCRAFTSQUAD", ANTIAIRCRAFTSQUAD}, {"MORTARSQUAD", MORTARSQUAD}, {"ENGINEER", ENGINEER}, {"SPECIALFORCES", SPECIALFORCES}, {"REGULARINFANTRY", REGULARINFANTRY}};
    auto vIt = vehicleMap.find(name);
    if (vIt != vehicleMap.end())
    {
        vType = vIt->second;
        isVehicle = true;
        return true;
    }
    auto iIt = infantryMap.find(name);
    if (iIt != infantryMap.end())
    {
        iType = iIt->second;
        isVehicle = false;
        return true;
    }
    return false;
}
Unit::Unit(int quantity, int weight, const Position pos)
    : quantity(quantity), weight(weight), pos(pos) {}

Unit::~Unit() {}

void Unit::setQuantity(int quan)
{
    quantity = quan;
}

void Unit::setWeight(int w)
{
    weight = w;
}

int Unit::getWeight() const { return weight; }

int Unit::getQuantity() const { return quantity; }

void Unit::setPos(Position p)
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
    unitList = new UnitList(LF, EXP);
    for (int i = 0; i < size; i++)
    {
        unitList->insert(unitArray[i]);
        if (Vehicle *vehicle = dynamic_cast<Vehicle *>(unitArray[i]))
        {
            LF += vehicle->getAttackScore();
        }
        else if (Infantry *infantry = dynamic_cast<Infantry *>(unitArray[i]))
        {
            EXP += infantry->getAttackScore();
        }
    }
    if (LF > 1000)
        LF = 1000;
    if (EXP > 500)
        EXP = 500;
}

Army::~Army() { delete unitList; }

int Army::getLF() { return LF; }

int Army::getExp() { return EXP; }

void Army::setLF(int lf) { LF = lf; }

void Army::setExp(int exp) { EXP = exp; }

// LiberationArmy
LiberationArmy::LiberationArmy(Unit **unitArray, int size, string name, BattleField *battleField) : Army(unitArray, size, name, battleField) {}

string LiberationArmy::str() const
{
    return "LiberationArmy: " + name + " (LF=" + to_string(LF) + ", EXP=" + to_string(EXP) + ")";
}

LiberationArmy::~LiberationArmy()
{ // unitList is deleted by base class Army
}

void LiberationArmy::removeUnits(vector<Unit *> &units)
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
UnitList::UnitList(int armyLF, int armyEXP) : head(nullptr), tail(nullptr), size(0)
{
    int S = armyLF + armyEXP;
    bool isSpecial = false;
    for (int k : {3, 5, 7})
    {
        if (isSpecialNumber(S, k))
        {
            isSpecial = true;
            break;
        }
    }
    capacity = isSpecial ? 12 : 8;
}

UnitList::~UnitList()
{
    clear();
}

bool UnitList::isContain(VehicleType vehicleType)
{
    Node *current = head;
    while (current)
    {
        if (Vehicle *vehicle = dynamic_cast<Vehicle *>(current->unit))
        {
            if (vehicle->getStringType() == vehicleTypeToString(vehicleType))
            {
                return true;
            }
        }
        current = current->next;
    }
    return false;
}

bool UnitList::isContain(InfantryType infantryType)
{
    Node *current = head;
    while (current)
    {
        if (Infantry *infantry = dynamic_cast<Infantry *>(current->unit))
        {
            if (infantry->getStringType() == infantryTypeToString(infantryType))
            {
                return true;
            }
        }
        current = current->next;
    }
    return false;
}

bool UnitList::insert(Unit *unit)
{
    if (!unit || size >= capacity)
        return false;
    Node *current = head;
    while (current)
    {
        bool sameType = false;
        bool samePos = current->unit->getCurrentPosition().str() == unit->getCurrentPosition().str();
        if (Vehicle *v1 = dynamic_cast<Vehicle *>(current->unit))
        {
            if (Vehicle *v2 = dynamic_cast<Vehicle *>(unit))
            {
                sameType = v1->getStringType() == v2->getStringType();
            }
        }
        else if (Infantry *i1 = dynamic_cast<Infantry *>(current->unit))
        {
            if (Infantry *i2 = dynamic_cast<Infantry *>(unit))
            {
                sameType = i1->getStringType() == i2->getStringType();
            }
        }
        if (sameType && samePos)
        {
            current->unit->setQuantity(current->unit->getQuantity() + unit->getQuantity());
            return true;
        }
        current = current->next;
    }

    Node *newNode = new Node(unit);
    if (dynamic_cast<Infantry *>(unit))
    {
        newNode->next = head;
        head = newNode;
        if (!tail)
            tail = newNode;
    }
    else if (dynamic_cast<Vehicle *>(unit))
    {
        if (!head)
        {
            head = tail = newNode;
        }
        else
        {
            tail->next = newNode;
            tail = newNode;
        }
    }
    else
    {
        delete newNode;
        return false;
    }
    size++;
    return true;
}

string UnitList::str() const
{
    stringstream ss;
    int vehicleCount = 0;
    int infantryCount = 0;

    Node *current = head;
    while (current)
    {
        if (dynamic_cast<Vehicle *>(current->unit))
        {
            vehicleCount++;
        }
        else if (dynamic_cast<Infantry *>(current->unit))
        {
            infantryCount++;
        }
        current = current->next;
    }

    ss << "UnitList[count_vehicle=" << vehicleCount
       << ";count_infantry=" << infantryCount << ";";

    current = head;
    for (int i = 0; current; i++, current = current->next)
    {
        ss << current->unit->str();
        if (current->next)
            ss << ",";
    }

    ss << "]";
    return ss.str();
}
vector<Unit *> UnitList::getAllUnits() const
{
    vector<Unit *> result;
    Node *current = head;
    while (current)
    {
        result.push_back(current->unit);
        current = current->next;
    }
    return result;
}

void UnitList::removeUnit(Unit *unit)
{
    Node *current = head;
    Node *prev = nullptr;
    while (current)
    {
        if (current->unit == unit)
        {
            if (prev)
            {
                prev->next = current->next;
            }
            else
            {
                head = current->next;
            }
            if (current == tail)
            {
                tail = prev;
            }
            delete current;
            size--;
            return;
        }
        prev = current;
        current = current->next;
    }
}

void UnitList::
{
    Node *current = head;
    while (current)
    {
        Node *next = current->next;
        delete current->unit; // Assume ownership
        delete current;
        current = next;
    }
    head = tail = nullptr;
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

// Position
Position::Position(int r, int c) : r(r), c(c) {}
Position::Position(const string &str_pos)
{
    string cleaned = str_pos;
    cleaned.erase(remove(cleaned.begin(), cleaned.end(), ' '), cleaned.end());
    if (cleaned.size() < 3 || cleaned[0] != '(' || cleaned[cleaned.size() - 1] != ')')
    {
        r = 0;
        c = 0;
        return;
    }
    string inner = cleaned.substr(1, cleaned.size() - 2); // Remove ( and )
    size_t comma = inner.find(',');
    if (comma == string::npos)
    {
        r = 0;
        c = 0;
        return;
    }
    try
    {
        r = stoi(inner.substr(0, comma));
        c = stoi(inner.substr(comma + 1));
    }
    catch (...)
    {
        r = 0;
        c = 0;
    }
}

int Position::getRow() const
{
    return r;
}

int Position::getCol() const
{
    return c;
}

void Position::setRow(int r)
{
    this->r = r;
}

void Position::setCol(int c)
{
    this->c = c;
}

string Position::str() const
{
    stringstream ss;
    ss << "(" << r << "," << c << ")";
    return ss.str();
}

// ARVN
ARVN::ARVN(const Unit **unitArray, int size, string name, BattleField *battleField)
    : Army(const_cast<Unit **>(unitArray), size, name, battleField)
{
}
void ARVN::fight(Army *enemy, bool defense)
{
    if (!enemy || !unitList)
        return;

    if (!defense)
    {
        vector<Unit *> toRemove;
        for (Unit *unit : unitList->getAllUnits())
        {
            int currentQuantity = unit->getQuantity();
            if (currentQuantity <= 1)
            {
                toRemove.push_back(unit);
                continue;
            }
            int newQuantity = static_cast<int>(currentQuantity * 0.8);
            if (newQuantity < 1)
                newQuantity = 1;
            unit->setQuantity(newQuantity);
            if (newQuantity == 1)
            {
                toRemove.push_back(unit);
            }
        }
        for (Unit *unit : toRemove)
        {
            unitList->removeUnit(unit);
        }
    }
    else
    {
        LiberationArmy *libArmy = dynamic_cast<LiberationArmy *>(enemy);
        if (libArmy)
        {
            libArmy->fight(this, false);
            if (unitList->getAllUnits().empty())
            {
                setLF(0);
                setEXP(0);
                return;
            }
            for (Unit *unit : unitList->getAllUnits())
            {
                int currentWeight = unit->getWeight();
                int newWeight = static_cast<int>(currentWeight * 0.8);
                if (newWeight < 1)
                    newWeight = 1;
                unit->setWeight(newWeight);
            }
        }
    }
    LF = 0;
    EXP = 0;
    for (Unit *unit : unitList->getAllUnits())
    {
        if (Vehicle *vehicle = dynamic_cast<Vehicle *>(unit))
        {
            LF += vehicle->getAttackScore();
        }
        else if (Infantry *infantry = dynamic_cast<Infantry *>(unit))
        {
            EXP += infantry->getAttackScore();
        }
    }
    if (LF > 1000)
        LF = 1000;
    if (LF < 0)
        LF = 0;
    if (EXP > 500)
        EXP = 500;
    if (EXP < 0)
        EXP = 0;
}

string ARVN::str() const
{
    stringstream ss;
    ss << "ARVN[name=" << name
       << ",LF=" << LF
       << ",EXP=" << EXP
       << ",unitList=" << (unitList ? unitList->str() : "null")
       << ",battleField=" << battleField
       << "]";
    return ss.str();
}
ARVN::~ARVN()
{
    // Army base class handles unitList deletion
}

TerrainElement::TerrainElement() {}
TerrainElement::~TerrainElement() {}
Road::Road()
{
}

void Road::getEffect(Army *army)
{
    // No effect on LiberationArmy or ARVN
}

Mountain::Mountain(const Position &pos) : pos(pos)
{
}

void Mountain::getEffect(Army *army)
{
    if (!army || !army->getUnitList())
        return;

    int deltaLF = 0;
    int deltaEXP = 0;
    int radius = 0;
    float expMultiplier = 0.0f;
    float lfMultiplier = 0.0f;

    // Determine army type and set parameters
    if (dynamic_cast<LiberationArmy *>(army))
    {
        radius = 2;
        expMultiplier = 0.3f;
        lfMultiplier = -0.1f;
    }
    else if (dynamic_cast<ARVN *>(army))
    {
        radius = 4;
        expMultiplier = -0.2f;
        lfMultiplier = -0.05f;
    }
    else
    {
        return;
    }
    for (Unit *unit : army->getUnitList()->getAllUnits())
    {
        if (euclideanDistance(unit->getCurrentPosition(), pos) <= radius)
        {
            if (Infantry *infantry = dynamic_cast<Infantry *>(unit))
            {
                deltaEXP += static_cast<int>(infantry->getAttackScore() * expMultiplier);
            }
            else if (Vehicle *vehicle = dynamic_cast<Vehicle *>(unit))
            {
                deltaLF += static_cast<int>(vehicle->getAttackScore() * lfMultiplier);
            }
        }
    }

    int newLF = army->getLF() + deltaLF;
    int newEXP = army->getEXP() + deltaEXP;
    if (newLF > 1000)
        newLF = 1000;
    if (newLF < 0)
        newLF = 0;
    if (newEXP > 500)
        newEXP = 500;
    if (newEXP < 0)
        newEXP = 0;
    army->setLF(newLF);
    army->setEXP(newEXP);
}

River::River(const Position &pos) : pos(pos) {}

void River::getEffect(Army *army)
{
    if (!army || !army->getUnitList())
        return;

    int deltaEXP = 0;
    const int radius = 2;
    const float multiplier = -0.1f; // 10% reduction

    for (Unit *unit : army->getUnitList()->getAllUnits())
    {
        if (euclideanDistance(unit->getCurrentPosition(), pos) <= radius)
        {
            if (Infantry *infantry = dynamic_cast<Infantry *>(unit))
            {
                deltaEXP += static_cast<int>(infantry->getAttack0444Score() * multiplier);
            }
        }
    }

    int newEXP = army->getEXP() + deltaEXP;
    if (newEXP > 500)
        newEXP = 500;
    if (newEXP < 0)
        newEXP = 0;
    army->setEXP(newEXP);
}

Urban::Urban(const Position &pos) : pos(pos) {}

void Urban::getEffect(Army *army)
{
    if (!army || !army->getUnitList())
        return;

    int deltaLF = 0;
    int deltaEXP = 0;

    if (LiberationArmy *libArmy = dynamic_cast<LiberationArmy *>(army))
    {
        for (Unit *unit : army->getUnitList()->getAllUnits())
        {
            int distance = euclideanDistance(unit->getCurrentPosition(), pos);
            if (Infantry *infantry = dynamic_cast<Infantry *>(unit))
            {
                string type = infantry->getStringType();
                if ((type == "SPECIALFORCES" || type == "REGULARINFANTRY") && distance <= 5)
                {
                    int D = distance == 0 ? 1 : distance;
                    deltaEXP += static_cast<int>(2.0 * infantry->getAttackScore() / D);
                }
            }
            else if (Vehicle *vehicle = dynamic_cast<Vehicle *>(unit))
            {
                if (vehicle->getStringType() == "ARTILLERY" && distance <= 2)
                {
                    deltaLF += static_cast<int>(vehicle->getAttackScore() * -0.5f); // 50% reduction
                }
            }
        }
    }
    else if (dynamic_cast<ARVN *>(army))
    {
        for (Unit *unit : army->getUnitList()->getAllUnits())
        {
            int distance = euclideanDistance(unit->getCurrentPosition(), pos);
            if (Infantry *infantry = dynamic_cast<Infantry *>(unit))
            {
                if (infantry->getStringType() == "REGULARINFANTRY" && distance <= 3)
                {
                    int D = distance == 0 ? 1 : distance;
                    deltaEXP += static_cast<int>(3.0 * infantry->getAttackScore() / (2.0 * D));
                }
            }
        }
    }

    int newLF = army->getLF() + deltaLF;
    int newEXP = army->getEXP() + deltaEXP;
    if (newLF > 1000)
        newLF = 1000;
    if (newLF < 0)
        newLF = 0;
    if (newEXP > 500)
        newEXP = 500;
    if (newEXP < 0)
        newEXP = 0;
    army->setLF(newLF);
    army->setEXP(newEXP);
}

Fortification::Fortification(const Position &pos) : pos(pos)
{
}

void Fortification::getEffect(Army *army)
{
    if (!army || !army->getUnitList())
        return;

    int deltaLF = 0;
    int deltaEXP = 0;
    const int radius = 2;
    float multiplier = 0.0f;

    if (dynamic_cast<LiberationArmy *>(army))
    {
        multiplier = -0.2f;
    }
    else if (dynamic_cast<ARVN *>(army))
    {
        multiplier = 0.2f;
    }
    else
    {
        return;
    }

    for (Unit *unit : army->getUnitList()->getAllUnits())
    {
        if (euclideanDistance(unit->getCurrentPosition(), pos) <= radius)
        {
            if (Infantry *infantry = dynamic_cast<Infantry *>(unit))
            {
                deltaEXP += static_cast<int>(infantry->getAttackScore() * multiplier);
            }
            else if (Vehicle *vehicle = dynamic_cast<Vehicle *>(unit))
            {
                deltaLF += static_cast<int>(vehicle->getAttackScore() * multiplier);
            }
        }
    }

    int newLF = army->getLF() + deltaLF;
    int newEXP = army->getEXP() + deltaEXP;
    if (newLF > 1000)
        newLF = 1000;
    if (newLF < 0)
        newLF = 0;
    if (newEXP > 500)
        newEXP = 500;
    if (newEXP < 0)
        newEXP = 0;
    army->setLF(newLF);
    army->setEXP(newEXP);
}

SpecialZone::SpecialZone(const Position &pos) : pos(pos)
{
}

void SpecialZone::getEffect(Army *army)
{
    if (!army || !army->getUnitList())
        return;

    int deltaLF = 0;
    int deltaEXP = 0;
    const int radius = 1;

    for (Unit *unit : army->getUnitList()->getAllUnits())
    {
        if (euclideanDistance(unit->getCurrentPosition(), pos) <= radius)
        {
            if (Infantry *infantry = dynamic_cast<Infantry *>(unit))
            {
                deltaEXP -= infantry->getAttackScore();
            }
            else if (Vehicle *vehicle = dynamic_cast<Vehicle *>(unit))
            {
                deltaLF -= vehicle->getAttackScore();
            }
        }
    }

    int newLF = army->getLF() + deltaLF;
    int newEXP = army->getEXP() + deltaEXP;
    if (newLF > 1000)
        newLF = 1000;
    if (newLF < 0)
        newLF = 0;
    if (newEXP > 500)
        newEXP = 500;
    if (newEXP < 0)
        newEXP = 0;
    army->setLF(newLF);
    army->setEXP(newEXP);
}

BattleField::BattleField(int n_rows, int n_cols, vector<Position *> arrayForest,
                         vector<Position *> arrayRiver, vector<Position *> arrayFortification,
                         vector<Position *> arrayUrban, vector<Position *> arraySpecialZone)
    : n_rows(n_rows >= 0 ? n_rows : 0), n_cols(n_cols >= 0 ? n_cols : 0)
{
    terrain = new TerrainElement *[n_rows];
    for (int i = 0; i < n_rows; i++)
    {
        terrain[i] = new TerrainElement *[n_cols];
        for (int j = 0; j < n_cols; j++)
        {
            terrain[i][j] = new Road();
        }
    }

    auto assignTerrain = [&](Position *pos, TerrainElement *element)
    {
        if (pos && pos->getRow() >= 0 && pos->getRow() < n_rows &&
            pos->getCol() >= 0 && pos->getCol() < n_cols)
        {
            delete terrain[pos->getRow()][pos->getCol()];
            terrain[pos->getRow()][pos->getCol()] = element;
        }
        else
        {
            delete element;
        }
    };

    for (Position *pos : arrayForest)
    {
        assignTerrain(pos, new Mountain(*pos));
    }
    for (Position *pos : arrayRiver)
    {
        assignTerrain(pos, new River(*pos));
    }
    for (Position *pos : arrayFortification)
    {
        assignTerrain(pos, new Fortification(*pos));
    }
    for (Position *pos : arrayUrban)
    {
        assignTerrain(pos, new Urban(*pos));
    }
    for (Position *pos : arraySpecialZone)
    {
        assignTerrain(pos, new SpecialZone(*pos));
    }
}

string BattleField::str() const
{
    stringstream ss;
    ss << "BattleField[n_rows=" << n_rows << ",n_cols=" << n_cols << "]";
    return ss.str();
}

BattleField::~BattleField()
{
    for (int i = 0; i < n_rows; i++)
    {
        for (int j = 0; j < n_cols; j++)
        {
            delete terrain[i][j];
        }
        delete[] terrain[i];
    }
    delete[] terrain;
}

// Configuration
Configuration::Configuration(const string &filepath)
    : num_rows(0), num_cols(0),
      liberationUnits(nullptr), liberationUnitsSize(0),
      ARVNUnits(nullptr), ARVNUnitsSize(0),
      eventCode(0)
{
    ifstream file(filepath);
    if (!file.is_open())
        return;

    string line;
    vector<Unit *> libUnits, arvnUnits;
    while (getline(file, line))
    {
        line = trim(line);
        if (line.empty())
            continue;

        size_t eqPos = line.find('=');
        if (eqPos == string::npos)
            continue;
        string key = trim(line.substr(0, eqPos));
        string value = trim(line.substr(eqPos + 1));

        if (key == "NUM_ROWS")
        {
            try
            {
                num_rows = stoi(value);
            }
            catch (...)
            {
            }
        }
        else if (key == "NUM_COLS")
        {
            try
            {
                num_cols = stoi(value);
            }
            catch (...)
            {
            }
        }
        else if (key == "ARRAY_FOREST")
        {
            arrayForest = parseTerrainArray(value);
        }
        else if (key == "ARRAY_RIVER")
        {
            arrayRiver = parseTerrainArray(value);
        }
        else if (key == "ARRAY_FORTIFICATION")
        {
            arrayFortification = parseTerrainArray(value);
        }
        else if (key == "ARRAY_URBAN")
        {
            arrayUrban = parseTerrainArray(value);
        }
        else if (key == "ARRAY_SPECIAL_ZONE")
        {
            arraySpecialZone = parseTerrainArray(value);
        }
        else if (key == "UNIT_LIST")
        {
            if (value.size() < 2 || value[0] != '[' || value[value.size() - 1] != ']')
                continue;
            string inner = trim(value.substr(1, value.size() - 2));
            if (inner.empty())
                continue;

            stringstream ss(inner);
            string unitStr;
            while (getline(ss, unitStr, ','))
            {
                Unit *unit = parseUnit(unitStr);
                if (unit)
                {
                    string posStr = unit->getCurrentPosition().str();
                    int armyBelong = (posStr == "(1,2)" || posStr == "(1,1)") ? 0 : 1; // Example logic
                    if (armyBelong == 0)
                    {
                        libUnits.push_back(unit);
                    }
                    else
                    {
                        arvnUnits.push_back(unit);
                    }
                }
            }
        }
        else if (key == "EVENT_CODE")
        {
            try
            {
                eventCode = stoi(value);
                if (eventCode < 0)
                    eventCode = 0;
                if (eventCode > 99)
                    eventCode = eventCode % 100;
            }
            catch (...)
            {
            }
        }
    }
    file.close();

    liberationUnitsSize = libUnits.size();
    if (liberationUnitsSize > 0)
    {
        liberationUnits = new Unit *[liberationUnitsSize];
        for (int i = 0; i < liberationUnitsSize; i++)
        {
            liberationUnits[i] = libUnits[i];
        }
    }
    ARVNUnitsSize = arvnUnits.size();
    if (ARVNUnitsSize > 0)
    {
        ARVNUnits = new Unit *[ARVNUnitsSize];
        for (int i = 0; i < ARVNUnitsSize; i++)
        {
            ARVNUnits[i] = arvnUnits[i];
        }
    }
}

string Configuration::str() const
{
    stringstream ss;
    ss << "Configuration[";
    ss << "num_rows=" << num_rows << ",";
    ss << "num_cols=" << num_cols << ",";

    auto printArray = [&](const vector<Position *> &arr, const string &name)
    {
        ss << name << "=[";
        for (size_t i = 0; i < arr.size(); i++)
        {
            ss << arr[i]->str();
            if (i < arr.size() - 1)
                ss << ",";
        }
        ss << "],";
    };
    printArray(arrayForest, "arrayForest");
    printArray(arrayRiver, "arrayRiver");
    printArray(arrayFortification, "arrayFortification");
    printArray(arrayUrban, "arrayUrban");
    printArray(arraySpecialZone, "arraySpecialZone");

    ss << "liberationUnits=[";
    for (int i = 0; i < liberationUnitsSize; i++)
    {
        ss << liberationUnits[i]->str();
        if (i < liberationUnitsSize - 1)
            ss << ",";
    }
    ss << "],";
    ss << "ARVNUnits=[";
    for (int i = 0; i < ARVNUnitsSize; i++)
    {
        ss << ARVNUnits[i]->str();
        if (i < ARVNUnitsSize - 1)
            ss << ",";
    }
    ss << "],";

    ss << "eventCode=" << eventCode;
    ss << "]";
    return ss.str();
}

Configuration::~Configuration()
{
    for (Position *pos : arrayForest)
        delete pos;
    for (Position *pos : arrayRiver)
        delete pos;
    for (Position *pos : arrayFortification)
        delete pos;
    for (Position *pos : arrayUrban)
        delete pos;
    for (Position *pos : arraySpecialZone)
        delete pos;

    for (int i = 0; i < liberationUnitsSize; i++)
    {
        delete liberationUnits[i];
    }
    delete[] liberationUnits;
    for (int i = 0; i < ARVNUnitsSize; i++)
    {
        delete ARVNUnits[i];
    }
    delete[] ARVNUnits;
}
////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////