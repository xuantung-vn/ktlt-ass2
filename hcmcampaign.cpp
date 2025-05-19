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

string trim(const string &s)
{
    string result = s;
    size_t start = 0;
    while (start < result.size() && (result[start] == ' ' || result[start] == '\t'))
        start++;
    result.erase(0, start);

    size_t end = result.size();
    while (end > 0 && (result[end - 1] == ' ' || result[end - 1] == '\t'))
        end--;
    result.erase(end);
    return result;
}

double euclideanDistance(const Position &p1, const Position &p2)
{
    int dr = p1.getRow() - p2.getRow();
    int dc = p1.getCol() - p2.getCol();
    return sqrt(dr * dr + dc * dc);
}

bool getUnitType(const string &name, VehicleType &vType, InfantryType &iType, bool &isVehicle)
{
    string upperName = name;
    for (char &c : upperName)
        c = toupper(c);
    const pair<string, VehicleType> vehiclePairs[] = {
        {"TRUCK", TRUCK}, {"MORTAR", MORTAR}, {"ANTIAIRCRAFT", ANTIAIRCRAFT}, {"ARMOREDCAR", ARMOREDCAR}, {"APC", APC}, {"ARTILLERY", ARTILLERY}, {"TANK", TANK}};
    const pair<string, InfantryType> infantryPairs[] = {
        {"SNIPER", SNIPER}, {"ANTIAIRCRAFTSQUAD", ANTIAIRCRAFTSQUAD}, {"MORTARSQUAD", MORTARSQUAD}, {"ENGINEER", ENGINEER}, {"SPECIALFORCES", SPECIALFORCES}, {"REGULARINFANTRY", REGULARINFANTRY}};
    for (const auto &pair : vehiclePairs)
    {
        if (upperName == pair.first)
        {
            vType = pair.second;
            isVehicle = true;
            return true;
        }
    }
    for (const auto &pair : infantryPairs)
    {
        if (upperName == pair.first)
        {
            iType = pair.second;
            isVehicle = false;
            return true;
        }
    }
    return false;
}

// Parse a single unit string and return a Unit
Unit *parseUnit(const string &unitStr, bool isLebsArmy)
{
    int quantity, armyBelong;
    float weight;
    string unitName;
    float posX, posY;

    size_t nameEnd = unitStr.find('(');
    unitName = unitStr.substr(0, nameEnd);

    string content = unitStr.substr(nameEnd + 1, unitStr.length() - nameEnd - 2);
    stringstream ss(content);
    string token;

    // Get quantity
    getline(ss, token, ',');
    quantity = stoi(token);

    // Get weight
    getline(ss, token, ',');
    weight = stof(token);

    // Get (posX, posY)
    getline(ss, token, ',');
    if (token.front() == ' ')
        token = token.substr(1);
    token = token.substr(1); // remove '('
    posX = stof(token);

    getline(ss, token, ')');
    if (token.front() == ' ')
        token = token.substr(1);
    posY = stof(token);

    // Get armyBelong
    getline(ss, token, ',');
    getline(ss, token); // Final value
    if (token.front() == ' ')
        token = token.substr(1);
    armyBelong = stoi(token);

    // Create Position
    Position *pos = new Position(posX, posY);

    // Determine unit type
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
        unit = new Vehicle(quantity, weight, *pos, vType);
    }
    else
    {
        unit = new Infantry(quantity, weight, *pos, iType);
    }

    if (armyBelong == 0)
    {
        isLebsArmy = true;
    }
    else if (armyBelong == 1)
    {
        isLebsArmy = false;
    }
    else
    {
        cout << "Error: Invalid armyBelong value: " << armyBelong << endl;
        delete unit;
        delete pos;
        return nullptr;
    }

    delete pos;
    return unit;
}

void parseStringToPosition(const string &key, string val, vector<Position *> &target)
{
    // Validate key using string comparison
    if (key != "ARRAY_FOREST" && key != "ARRAY_RIVER" &&
            key != "ARRAY_FORTIFICATION" && key != "ARRAY_URBAN" &&
            key != "ARRAY_SPECIAL_ZONE" ||
        val.empty())
    {
        return;
    }

    // Remove square brackets if present
    if (val.front() == '[')
        val.erase(0, 1);
    if (val.back() == ']')
        val.pop_back();

    size_t start = 0;
    while (start < val.size())
    {
        size_t left = val.find('(', start);
        size_t right = val.find(')', left);
        if (left == string::npos || right == string::npos)
            break;

        string pair = val.substr(left + 1, right - left - 1);
        size_t comma = pair.find(',');
        if (comma != string::npos)
        {
            int row = stoi(pair.substr(0, comma));
            int col = stoi(pair.substr(comma + 1));
            target.push_back(new Position(row, col));
        }
        start = right + 1;
        while (start < val.size() && (val[start] == ',' || isspace(val[start])))
        {
            ++start;
        }
    }
}
vector<Position *> parseTerrainArray(const string &value)
{
    vector<Position *> positions;
    string cleaned = trim(value);
    if (cleaned.empty() || cleaned.size() < 2 || cleaned[0] != '[' || cleaned.back() != ']')
    {
        return positions;
    }
    string inner = trim(cleaned.substr(1, cleaned.size() - 2));
    if (inner.empty())
    {
        return positions;
    }
    stringstream ss(inner);
    string posStr;
    while (getline(ss, posStr, ','))
    {
        posStr = trim(posStr);
        if (posStr.empty())
            continue;
        try
        {
            Position *pos = new Position(posStr);
            if (pos->getRow() != 0 || pos->getCol() != 0 || posStr == "(0,0)")
            {
                positions.push_back(pos);
            }
            else
            {
                delete pos;
            }
        }
        catch (...)
        {
            continue;
        }
    }
    return positions;
}

// Unit
Unit::Unit(int quantity, int weight, const Position pos)
    : quantity(quantity), weight(weight), pos(pos) {}

Unit::~Unit() {}

int Unit::getAttackScore() { return 0; }

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
Vehicle::Vehicle(int quantity, int weight, const Position pos, VehicleType vehicleType)
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
    ss << "Vehicle[vehicleType=" << getStringType() << ",quantity=" << quantity
       << ",weight=" << weight << ",position=" << pos.str() << "]";
    return ss.str();
}

VehicleType Vehicle::getVehicleType() const { return vehicleType; }

// Infantry
Infantry::Infantry(int quantity, int weight, const Position pos, InfantryType infantryType)
    : Unit(quantity, weight, pos), infantryType(infantryType) {}

Infantry::~Infantry() {}

bool Infantry::isPerfectSquare(int n) const
{
    if (n < 0)
        return false;
    int root = static_cast<int>(sqrt(n));
    return root * root == n;
}

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
    ss << "Infantry[infantryType=" << getStringType() << ",quantity=" << quantity
       << ",weight=" << weight << ",position=" << pos.str() << "]";
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

UnitList *Army::getUnitList() const { return unitList; }

// LiberationArmy
LiberationArmy::LiberationArmy(Unit **unitArray, int size, string name, BattleField *battleField)
    : Army(unitArray, size, name, battleField) {}

string LiberationArmy::str() const
{
    stringstream ss;
    ss << "LiberationArmy[name=" << name
       << ",LF=" << LF
       << ",EXP=" << EXP
       << ",unitList=" << (unitList ? unitList->str() : "null")
       << ",battleField=" << battleField
       << "]";
    return ss.str();
}

LiberationArmy::~LiberationArmy() {}

void LiberationArmy::removeUnit(Unit *unit)
{
    unitList->removeUnit(unit);
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
    for (Unit *unit : enemy->getUnitList()->getAllUnits())
    {
        unitList->insert(unit);
    }
    enemy->getUnitList()->clear();
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
    if (!enemy || !unitList)
        return;

    float liberationLF = LF * (defense ? 1.3 : 1.5);
    float liberationEXP = EXP * (defense ? 1.3 : 1.5);
    float enemyLF = enemy->getLF();
    float enemyEXP = enemy->getExp();

    if (defense)
    {
        if (liberationLF >= enemyLF && liberationEXP >= enemyEXP)
        {
            confiscateEnemyUnits(enemy);
            recalcIndices();
            return;
        }
        if (liberationLF < enemyLF && liberationEXP < enemyEXP)
        {
            reinforceUnitsWithFibonacci();
            recalcIndices();
            liberationLF = getLF() * 1.3;
            liberationEXP = getExp() * 1.3;
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
            for (Unit *unit : combInfantry)
                removeUnit(unit);
            for (Unit *unit : combVehicle)
                removeUnit(unit);
            confiscateEnemyUnits(enemy);
        }
        else if (hasCombInfantry && liberationLF > enemyLF)
        {
            for (Unit *unit : combInfantry)
                removeUnit(unit);
            eliminateAllVehicles();
            confiscateEnemyUnits(enemy);
        }
        else if (hasCombVehicle && liberationEXP > enemyEXP)
        {
            for (Unit *unit : combVehicle)
                removeUnit(unit);
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

// UnitList
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

void UnitList::clear()
{
    Node *current = head;
    while (current)
    {
        Node *next = current->next;
        delete current->unit;
        delete current;
        current = next;
    }
    head = tail = nullptr;
    size = 0;
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
            if (vehicle->getVehicleType() == vehicleType)
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
            delete unit;
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
    string cleaned = trim(str_pos);
    if (cleaned.size() < 3 || cleaned[0] != '(' || cleaned.back() != ')')
    {
        r = c = 0;
        return;
    }
    cleaned = cleaned.substr(1, cleaned.size() - 2);
    stringstream ss(cleaned);
    string r_str, c_str;
    if (!getline(ss, r_str, ',') || !getline(ss, c_str))
    {
        r = c = 0;
        return;
    }
    try
    {
        r = stoi(trim(r_str));
        c = stoi(trim(c_str));
    }
    catch (...)
    {
        r = c = 0;
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
ARVN::ARVN(Unit **unitArray, int size, string name, BattleField *battleField)
    : Army(unitArray, size, name, battleField)
{
}

void ARVN::fight(Army *enemy, bool defense)
{
    if (!enemy || !unitList)
        return;
    float multiplier = defense ? 0.8f : 1.0f;
    for (Unit *unit : unitList->getAllUnits())
    {
        int score = unit->getAttackScore();
        int reduction = static_cast<int>(enemy->getExp() * 0.1);
        unit->setWeight(unit->getWeight() - reduction / unit->getQuantity());
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

ARVN::~ARVN() {}

// TerrainElement
TerrainElement::~TerrainElement() {}

Road::Road() {}

void Road::getEffect(Army *army)
{
    // No effect on LiberationArmy or ARVN
}

Mountain::Mountain(const Position &pos) : pos(pos) {}

void Mountain::getEffect(Army *army)
{
    if (!army || !army->getUnitList())
        return;

    int deltaLF = 0;
    int deltaEXP = 0;
    int radius = 0;
    float expMultiplier = 0.0f;
    float lfMultiplier = 0.0f;

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
    int newEXP = army->getExp() + deltaEXP;
    if (newLF > 1000)
        newLF = 1000;
    if (newLF < 0)
        newLF = 0;
    if (newEXP > 500)
        newEXP = 500;
    if (newEXP < 0)
        newEXP = 0;
    army->setLF(newLF);
    army->setExp(newEXP);
}

River::River(const Position &pos) : pos(pos) {}

void River::getEffect(Army *army)
{
    if (!army || !army->getUnitList())
        return;

    int deltaEXP = 0;
    const int radius = 2;
    const float multiplier = -0.1f;

    for (Unit *unit : army->getUnitList()->getAllUnits())
    {
        if (euclideanDistance(unit->getCurrentPosition(), pos) <= radius)
        {
            if (Infantry *infantry = dynamic_cast<Infantry *>(unit))
            {
                deltaEXP += static_cast<int>(infantry->getAttackScore() * multiplier);
            }
        }
    }

    int newEXP = army->getExp() + deltaEXP;
    if (newEXP > 500)
        newEXP = 500;
    if (newEXP < 0)
        newEXP = 0;
    army->setExp(newEXP);
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
                    deltaLF += static_cast<int>(vehicle->getAttackScore() * -0.5f);
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
    int newEXP = army->getExp() + deltaEXP;
    if (newLF > 1000)
        newLF = 1000;
    if (newLF < 0)
        newLF = 0;
    if (newEXP > 500)
        newEXP = 500;
    if (newEXP < 0)
        newEXP = 0;
    army->setLF(newLF);
    army->setExp(newEXP);
}

Fortification::Fortification(const Position &pos) : pos(pos) {}

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
    int newEXP = army->getExp() + deltaEXP;
    if (newLF > 1000)
        newLF = 1000;
    if (newLF < 0)
        newLF = 0;
    if (newEXP > 500)
        newEXP = 500;
    if (newEXP < 0)
        newEXP = 0;
    army->setLF(newLF);
    army->setExp(newEXP);
}

SpecialZone::SpecialZone(const Position &pos) : pos(pos) {}

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
    int newEXP = army->getExp() + deltaEXP;
    if (newLF > 1000)
        newLF = 1000;
    if (newLF < 0)
        newLF = 0;
    if (newEXP > 500)
        newEXP = 500;
    if (newEXP < 0)
        newEXP = 0;
    army->setLF(newLF);
    army->setExp(newEXP);
}

// BattleField
BattleField::BattleField(int n_rows, int n_cols,
                         vector<Position *> arrayForest,
                         vector<Position *> arrayRiver,
                         vector<Position *> arrayFortification,
                         vector<Position *> arrayUrban,
                         vector<Position *> arraySpecialZone)
    : n_rows(n_rows), n_cols(n_cols)
{
    if (n_rows <= 0 || n_cols <= 0)
    {
        throw invalid_argument("Invalid battlefield dimensions");
    }

    terrain = vector<vector<TerrainElement *>>(
        n_rows, vector<TerrainElement *>(n_cols, nullptr));
    for (int i = 0; i < n_rows; ++i)
    {
        for (int j = 0; j < n_cols; ++j)
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
    for (int i = 0; i < n_rows; ++i)
    {
        for (int j = 0; j < n_cols; ++j)
        {
            delete terrain[i][j];
        }
    }
}

const vector<vector<TerrainElement *>> &BattleField::getTerrain() const
{
    return terrain;
}

// Configuration
Configuration::Configuration(const string &filepath)
{
    num_rows = 0;
    num_cols = 0;
    eventCode = 0;

    ifstream file(filepath);
    if (!file.is_open())
    {
        return;
    }

    string line;
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
            num_rows = stoi(value);
        }
        else if (key == "NUM_COLS")
        {
            num_cols = stoi(value);
        }
        else if (key == "EVENT_CODE")
        {
            eventCode = stoi(value);
            if (eventCode < 0)
                eventCode = 0;
            if (eventCode > 99)
                eventCode = eventCode % 100;
        }
        else if (key == "ARRAY_FOREST")
        {
            arrayForest.clear();
            parseStringToPosition(key, value, arrayForest);
        }
        else if (key == "ARRAY_RIVER")
        {
            arrayRiver.clear();
            parseStringToPosition(key, value, arrayRiver);
        }
        else if (key == "ARRAY_FORTIFICATION")
        {
            arrayFortification.clear();
            parseStringToPosition(key, value, arrayFortification);
        }
        else if (key == "ARRAY_URBAN")
        {
            arrayUrban.clear();
            parseStringToPosition(key, value, arrayUrban);
        }
        else if (key == "ARRAY_SPECIAL_ZONE")
        {
            arraySpecialZone.clear();
            parseStringToPosition(key, value, arraySpecialZone);
        }
        else if (key == "UNIT_LIST")
        {
            string cleaned = trim(value);
            if (cleaned.empty() || cleaned.size() < 2 || cleaned[0] != '[' || cleaned.back() != ']')
            {
                continue;
            }
            string inner = trim(cleaned.substr(1, cleaned.size() - 2));
            if (inner.empty())
            {
                continue;
            }
            vector<string> unitStrings;
            stringstream ss(inner);
            string unitStr;
            size_t start = 0;
            int parenCount = 0;
            for (size_t i = 0; i < inner.size(); ++i)
            {
                if (inner[i] == '(')
                    parenCount++;
                else if (inner[i] == ')')
                    parenCount--;
                else if (inner[i] == ',' && parenCount == 0)
                {
                    unitStrings.push_back(trim(inner.substr(start, i - start)));
                    start = i + 1;
                }
            }
            if (start < inner.size())
            {
                unitStrings.push_back(trim(inner.substr(start)));
            }
            for (size_t i = 0; i < unitStrings.size(); ++i)
            {
                string unitStr = trim(unitStrings[i]);
                if (unitStr.empty())
                {
                    continue;
                }
                // Parse unit
                bool isLebsArmy = false;
                Unit *unit = parseUnit(unitStr, isLebsArmy);

                if (isLebsArmy)
                {
                    liberationUnits.push_back(unit);
                }
                else
                {
                    ARVNUnits.push_back(unit);
                }
                delete unit;
            }
        }
    }
    file.close();
}
string Configuration::str() const
{
    stringstream ss;
    ss << "Configuration[";
    ss << "num_rows=" << num_rows << ",";
    ss << "num_cols=" << num_cols << ",";

    ss << "arrayForest=[";
    for (size_t i = 0; i < arrayForest.size(); ++i)
    {
        ss << arrayForest[i]->str();
        if (i < arrayForest.size() - 1)
            ss << ",";
    }
    ss << "],";

    ss << "arrayRiver=[";
    for (size_t i = 0; i < arrayRiver.size(); ++i)
    {
        ss << arrayRiver[i]->str();
        if (i < arrayRiver.size() - 1)
            ss << ",";
    }
    ss << "],";

    ss << "arrayFortification=[";
    for (size_t i = 0; i < arrayFortification.size(); ++i)
    {
        ss << arrayFortification[i]->str();
        if (i < arrayFortification.size() - 1)
            ss << ",";
    }
    ss << "],";

    ss << "arrayUrban=[";
    for (size_t i = 0; i < arrayUrban.size(); ++i)
    {
        ss << arrayUrban[i]->str();
        if (i < arrayUrban.size() - 1)
            ss << ",";
    }
    ss << "],";

    ss << "arraySpecialZone=[";
    for (size_t i = 0; i < arraySpecialZone.size(); ++i)
    {
        ss << arraySpecialZone[i]->str();
        if (i < arraySpecialZone.size() - 1)
            ss << ",";
    }
    ss << "],";

    ss << "liberationUnits=[";
    bool first = true;
    for (const Unit *unit : liberationUnits)
    {
        if (unit)
        {
            if (!first)
                ss << ",";
            ss << unit->str();
            first = false;
        }
    }
    ss << "],";
    ss << "ARVNUnits=[";
    bool firstARVN = true;

    // for (const Unit *unit : ARVNUnits)
    // {
    //     if (unit)
    //     {
    //         if (!firstARVN)
    //             ss << ",";
    //         ss << unit->str();
    //         first = false;
    //     }
    // }
    ss << "],";

    ss << "eventCode=" << eventCode << "]";
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

    for (Unit *u : liberationUnits)
        delete u;
    for (Unit *u : ARVNUnits)
        delete u;
    liberationUnits.clear();
    ARVNUnits.clear();
}
vector<Position *> Configuration::getArrayForest() const
{
    return arrayForest;
}

vector<Position *> Configuration::getArrayRiver() const
{
    return arrayRiver;
}

vector<Position *> Configuration::getArrayFortification() const
{
    return arrayFortification;
}

vector<Position *> Configuration::getArrayUrban() const
{
    return arrayUrban;
}

vector<Position *> Configuration::getArraySpecialZone() const
{
    return arraySpecialZone;
}

vector<Unit *> Configuration::getLiberationUnits() const
{
    return liberationUnits;
}

vector<Unit *> Configuration::getARVNUnits() const
{
    return ARVNUnits;
}

// HCMCampaign
HCMCampaign::HCMCampaign(const string &config_file_path)
{
    config = new Configuration(config_file_path);

    battleField = new BattleField(
        config->getNumRows(),
        config->getNumCols(),
        config->getArrayForest(),
        config->getArrayRiver(),
        config->getArrayFortification(),
        config->getArrayUrban(),
        config->getArraySpecialZone());

    vector<Unit *> liberationUnits = config->getLiberationUnits();
    vector<Unit *> ARVNUnits = config->getARVNUnits();
    auto toPointerArray = [](const vector<Unit *> &vect) -> Unit **
    {
        if (vect.empty())
            return nullptr;
        Unit **arr = new Unit *[vect.size()];
        for (size_t i = 0; i < vect.size(); ++i)
        {
            arr[i] = vect[i];
        }
        return arr;
    };
    Unit **liberationArray = toPointerArray(liberationUnits);
    Unit **ARVNArray = toPointerArray(ARVNUnits);

    this->liberationArmy = new LiberationArmy(liberationArray, liberationUnits.size(), "LiberationArmy", this->battleField);
    this->arvnArmy = new ARVN(ARVNArray, ARVNUnits.size(), "ARVN", this->battleField);

    delete[] liberationArray;
    delete[] ARVNArray;
}

void HCMCampaign::run()
{
    if (!battleField || !liberationArmy || !arvnArmy || !config)
        return;

    const vector<vector<TerrainElement *>> &terrain = battleField->getTerrain();
    int n_rows = battleField->getRows();
    int n_cols = battleField->getCols();
    for (int i = 0; i < n_rows; i++)
    {
        for (int j = 0; j < n_cols; j++)
        {
            if (terrain[i][j])
            {
                terrain[i][j]->getEffect(liberationArmy);
                terrain[i][j]->getEffect(arvnArmy);
            }
        }
    }

    int eventCode = config->getEventCode();
    if (eventCode < 75)
    {
        liberationArmy->fight(arvnArmy, false);
    }
    else
    {
        arvnArmy->fight(liberationArmy, false);
        liberationArmy->fight(arvnArmy, false);
    }

    auto removeLowScoreUnits = [](Army *army)
    {
        if (!army || !army->getUnitList())
            return;
        vector<Unit *> toRemove;
        for (Unit *unit : army->getUnitList()->getAllUnits())
        {
            if (unit->getAttackScore() <= 5)
            {
                toRemove.push_back(unit);
            }
        }
        for (Unit *unit : toRemove)
        {
            army->getUnitList()->removeUnit(unit);
        }
    };
    removeLowScoreUnits(liberationArmy);
    removeLowScoreUnits(arvnArmy);

    auto updateIndices = [](Army *army)
    {
        int LF = 0, EXP = 0;
        for (Unit *unit : army->getUnitList()->getAllUnits())
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
        army->setLF(LF > 1000 ? 1000 : (LF < 0 ? 0 : LF));
        army->setExp(EXP > 500 ? 500 : (EXP < 0 ? 0 : EXP));
    };
    updateIndices(liberationArmy);
    updateIndices(arvnArmy);
}

string HCMCampaign::printResult()
{
    stringstream ss;
    ss << "LIBERATIONARMY[LF=" << liberationArmy->getLF()
       << ",EXP=" << liberationArmy->getExp() << "]-";
    ss << "ARVN[LF=" << arvnArmy->getLF()
       << ",EXP=" << arvnArmy->getExp() << "]";
    return ss.str();
}

HCMCampaign::~HCMCampaign()
{
    delete liberationArmy;
    delete arvnArmy;
    delete battleField;
    delete config;
}

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////