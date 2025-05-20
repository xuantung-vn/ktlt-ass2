#include "hcmcampaign.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
////////////////////////////////////////////////////////////////////////

string toUpper(const string &s)
{
    string result = s;
    for (char &c : result)
    {
        if (c >= 'a' && c <= 'z')
        {
            c = c - 'a' + 'A';
        }
    }
    return result;
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

double Position::euclideanDistance(const Position &p2)
{
    int dr = this->getRow() - p2.getRow();
    int dc = this->getCol() - p2.getCol();
    return sqrt(dr * dr + dc * dc);
}

bool getUnitType(const string &name, VehicleType &vType, InfantryType &iType, bool &isVehicle)
{
    string upperName = toUpper(name);
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

Unit *Configuration::parseUnit(const string &unitStr, bool &isLebsArmy)
{
    int quantity = 0;
    float weight = 0.0f;
    string unitName;
    float posX = 0.0f, posY = 0.0f;
    int armyBelong = -1;

    size_t nameEnd = unitStr.find('(');
    if (nameEnd == string::npos)
        return nullptr;
    unitName = unitStr.substr(0, nameEnd);

    string content = unitStr.substr(nameEnd + 1, unitStr.length() - nameEnd - 2);
    stringstream ss(content);
    string token;

    if (!getline(ss, token, ','))
        return nullptr;
    try
    {
        quantity = stoi(token);
    }
    catch (...)
    {
        return nullptr;
    }

    if (!getline(ss, token, ','))
        return nullptr;
    try
    {
        weight = stof(token);
    }
    catch (...)
    {
        return nullptr;
    }

    if (!getline(ss, token, ','))
        return nullptr;
    if (token.front() == ' ')
        token = token.substr(1);
    if (token.front() != '(')
        return nullptr;
    token = token.substr(1);
    try
    {
        posX = stof(token);
    }
    catch (...)
    {
        return nullptr;
    }

    if (!getline(ss, token, ')'))
        return nullptr;
    if (token.front() == ' ')
        token = token.substr(1);
    try
    {
        posY = stof(token);
    }
    catch (...)
    {
        return nullptr;
    }

    if (!getline(ss, token, ',') || !getline(ss, token))
        return nullptr;
    if (token.front() == ' ')
        token = token.substr(1);
    try
    {
        armyBelong = stoi(token);
    }
    catch (...)
    {
        return nullptr;
    }

    Position *pos = new Position(posX, posY);

    VehicleType vType;
    InfantryType iType = REGULARINFANTRY;
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

Unit::Unit(int quantity, int weight, const Position pos)
    : quantity(quantity), weight(weight), pos(pos) {}

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

Unit::~Unit() {}
// Unit END HERE

// Vehicle START HERE
Vehicle::Vehicle(int quantity, int weight, const Position pos, VehicleType vehicleType)
    : Unit(quantity, weight, pos), vehicleType(vehicleType) {}

string Vehicle::className() const
{
    stringstream ss;
    ss << "Vehicle";
    return ss.str();
}
int Vehicle::getAttackScore()
{
    return ceil(1.0 * (static_cast<int>(vehicleType) * 304 + quantity * weight) / 30);
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

Unit *Vehicle::clone() const
{
    return new Vehicle(this->quantity, this->weight, this->pos, this->vehicleType);
}

Vehicle::~Vehicle() {}
// Vehicle END HERE

// Infantry START HERE
Infantry::Infantry(int quantity, int weight, const Position pos, InfantryType infantryType)
    : Unit(quantity, weight, pos), infantryType(infantryType) {}
string Infantry::className() const
{
    stringstream ss;

    ss << "Infantry";
    return ss.str();
}
bool Infantry::isPerfectSquare(int n) const
{
    if (n < 0)
        return false;
    int root = ceil(sqrt(n));
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
    int typeValue = int(infantryType);
    int initialScore = typeValue * 56 + quantity * weight;
    int extraScore = SPECIALFORCES && isPerfectSquare(weight) ? 75 : 0;
    int pNumber = personalNumber(initialScore, 1975);
    if (pNumber > 7)
    {
        quantity = (int)ceil(1.2 * this->quantity);
    }
    else if (pNumber < 3)
    {
        quantity = (int)ceil(0.9 * this->quantity);
        if (this->quantity < 0)
            this->quantity = 0;
    }
    return typeValue * 56 + quantity * weight + extraScore;
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

Unit *Infantry ::clone() const
{
    return new Infantry(this->quantity, this->weight, this->pos, this->infantryType);
}

string Infantry::str() const
{
    stringstream ss;
    ss << "Infantry[infantryType=" << getStringType() << ",quantity=" << quantity
       << ",weight=" << weight << ",position=" << pos.str() << "]";
    return ss.str();
}

Infantry::~Infantry() {}

// Infantry END HERE

// Army START HERE
Army::Army(Unit **unitArray, int size, string name, BattleField *battleField)
    : name(name), battleField(battleField), LF(0), EXP(0)
{
    if (size < 0)
        size = 0;
    this->name = name;
    this->battleField = battleField;
    this->LF = 0;
    this->EXP = 0;

    unitList = new UnitList(LF, EXP);
    for (int i = 0; i < size && unitArray; i++)
    {
        if (unitArray[i])
        {
            Unit *unitCopy = nullptr;
            if (Vehicle *vehicle = dynamic_cast<Vehicle *>(unitArray[i]))
            {
                unitCopy = new Vehicle(*vehicle);
                LF += unitCopy->getAttackScore();
            }
            else if (Infantry *infantry = dynamic_cast<Infantry *>(unitArray[i]))
            {
                unitCopy = new Infantry(*infantry);
                EXP += unitCopy->getAttackScore();
            }
            if (unitCopy)
            {
                if (!unitList->insert(unitCopy))
                {
                    delete unitCopy;
                }
            }
        }
    }
    LF = min(LF, 1000);
    EXP = min(EXP, 500);
}

int Army::getLF() { return LF; }

int Army::getExp() { return EXP; }

void Army::setLF(int lf) { LF = lf; }

void Army::setExp(int exp) { EXP = exp; }

UnitList *Army::getUnitList() const { return unitList; }

Army::~Army() { delete unitList; }
// Army END HERE

// LiberationArmy START HERE
LiberationArmy::LiberationArmy(Unit **unitArray, int size, string name, BattleField *battleField)
    : Army(unitArray, size, name, battleField) {}

string LiberationArmy::str() const
{
    stringstream ss;
    ss << "LiberationArmy["
       << "LF=" << LF
       << ",EXP=" << EXP
       << ",unitList=" << (unitList ? unitList->str() : "null")
       << ",battleField=" + ((this->battleField != nullptr) ? this->battleField->str() : "]");
    return ss.str();
}

void Army::removeUnit(Unit *unit)
{
    unitList->removeUnit(unit);
}

void Army::recalcIndices()
{
    if (!unitList || !unitList->getHead())
    {
        LF = 0;
        EXP = 0;
        return;
    }
    int vehicleScore = 0, infantryScore = 0;
    for (Unit *unit : unitList->getAllUnits())
    {
        if (Vehicle *vehicle = dynamic_cast<Vehicle *>(unit))
        {
            vehicleScore += vehicle->getAttackScore();
        }
        else if (Infantry *infantry = dynamic_cast<Infantry *>(unit))
        {
            infantryScore += infantry->getAttackScore();
        }
    }
    LF = min(1000, vehicleScore);
    EXP = min(500, infantryScore);
    int sum = LF + EXP;
    this->unitList->setCapacity(unitList->isSpecialNumber(sum) ? 12 : 8);
}

void Army::confiscateEnemyUnits(Army *enemy)
{
    if (!enemy || !enemy->getUnitList())
    {
        return;
    }
    Node *head1 = enemy->getUnitList()->getHead();
    vector<Unit *> dao_list;
    while (head1 != nullptr)
    {
        Unit *unitClone = head1->unit->clone();
        dao_list.push_back(unitClone);
        head1 = head1->next;
    }
    for (int i = dao_list.size() - 1; i >= 0; i--)
    {
        this->unitList->insert(dao_list[i]);
    }
    enemy->getUnitList()->setHead(nullptr);
}

vector<Unit *> LiberationArmy::findSmallestComb(const vector<Unit *> &units, int target)
{
    vector<Unit *> bestSubset;
    int minSum = INT_MAX;
    int n = units.size();
    int sum = 0;

    int left = 0;
    for (int right = 0; right < n; right++)
    {
        sum += units[right]->getAttackScore();
        Unit *unit_clone = units[right]->clone();
        bestSubset.push_back(unit_clone);
        if (sum > target)
        {
            while (sum - units[left]->getAttackScore() > target)
            {
                sum -= units[left++]->getAttackScore();
                bestSubset.erase(bestSubset.begin());
            }
            break;
        }
    }
    return bestSubset;
}
void Army::eliminateAllInfantry()
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

void Army::eliminateAllVehicles()
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

void Army::reduceAllUnitsWeight(int percentage)
{
    for (Unit *unit : unitList->getAllUnits())
    {
        int currentWeight = unit->getWeight();
        int newWeight = ceil(currentWeight * (100 - percentage) / 100);
        unit->setWeight(newWeight);
    }
}

int Army::nextFibonacci(int n)
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

void Army::reinforceUnitsWithFibonacci()
{
    for (Unit *unit : unitList->getAllUnits())
    {
        int q = unit->getQuantity();
        unit->setQuantity(nextFibonacci(q));
    }
}
void LiberationArmy::fight(Army *enemy, bool defense)
{
    if (!enemy || !unitList || !enemy->getUnitList() || unitList->getAllUnits().empty())
    {
        return;
    }

    float multiplier = defense ? 1.3f : 1.5f;
    float liberationLF = ceil(LF * multiplier);
    float liberationEXP = ceil(EXP * multiplier);
    LF = liberationLF;
    EXP = liberationEXP;

    int enemyLF = enemy->getLF();
    int enemyEXP = enemy->getExp();

    if (defense)
    {
        if (liberationLF >= enemyLF && liberationEXP >= enemyEXP)
        {
            // Both indices are sufficient: victory
            confiscateEnemyUnits(enemy);
            recalcIndices();
        }
        else if (liberationLF < enemyLF && liberationEXP < enemyEXP)
        {
            // Both indices are lower: reinforce with Fibonacci
            reinforceUnitsWithFibonacci();
            recalcIndices();
            // Re-evaluate after reinforcement
            liberationLF = ceil(LF * 1.3f);
            liberationEXP = ceil(EXP * 1.3f);
            LF = liberationLF;
            EXP = liberationEXP;
            if (liberationLF >= enemyLF && liberationEXP >= enemyEXP)
            {
                confiscateEnemyUnits(enemy);
                recalcIndices();
            }
            else
            {
                this->reduceAllUnitsWeight(10); // Reduce weight by 10%
                recalcIndices();
            }
        }
        else
        {
            // One index is lower: no loss, but reduce unit weights
            this->reduceAllUnitsWeight(10); // Reduce weight by 10%
            recalcIndices();
        }
    }
    else
    {
        vector<Unit *> tempInfantry; // vehicle
        vector<Unit *> tempVehicle;  // infantry
        Node *current = unitList->getHead();

        while (current != nullptr)
        {
            bool isVehicle = current->unit->className() == "Vehicle";
            if (!isVehicle)
            {
                tempInfantry.push_back(current->unit);
            }
            else if (isVehicle)
            {
                tempVehicle.push_back(current->unit);
            }
            current = current->next;
        }

        vector<Unit *> combInfantry = findSmallestComb(tempInfantry, enemyEXP);
        vector<Unit *> combVehicle = findSmallestComb(tempVehicle, enemyLF);

        bool hasCombInfantry = !combInfantry.empty();
        bool hasCombVehicle = !combVehicle.empty();

        if (hasCombInfantry && hasCombVehicle)
        {
            // Both combinations found: victory
            for (Unit *unit : combInfantry)
            {
                unitList->removeUnit(unit);
            }
            for (Unit *unit : combVehicle)
            {
                unitList->removeUnit(unit);
            }
            confiscateEnemyUnits(enemy);
            recalcIndices();
        }
        else if (hasCombInfantry && liberationLF > enemyLF)
        {
            // Infantry combination found and LF advantage: victory
            for (Unit *unit : combInfantry)
            {
                removeUnit(unit);
            }
            eliminateAllVehicles();
            confiscateEnemyUnits(enemy);
            recalcIndices();
        }
        else if (hasCombVehicle && liberationEXP > enemyEXP)
        {
            // Vehicle combination found and EXP advantage: victory
            for (Unit *unit : combVehicle)
            {
                removeUnit(unit);
            }
            eliminateAllInfantry();
            confiscateEnemyUnits(enemy);
            LF = enemyLF;
            EXP = enemyEXP;
            recalcIndices();
        }
        else
        {
            // No battle occurs: reduce unit weights by 10%
            this->reduceAllUnitsWeight(10);
            recalcIndices();
        }
    }
}
LiberationArmy::~LiberationArmy() {}
// LiberationArmy END HERE

// ARVN START HERE
ARVN::ARVN(Unit **unitArray, int size, string name, BattleField *battleField)
    : Army(unitArray, size, name, battleField)
{
}
void ARVN::clearAllUnits()
{
    Node *current = unitList->getHead();
    while (current != nullptr)
    {
        Node *toDelete = current;
        current = current->next;
        delete toDelete;
    }
    unitList->setHead(nullptr);
}
void ARVN::fight(Army *enemy, bool defense)
{
    if (!enemy || !unitList || !enemy->getUnitList() || !unitList->getHead())
    {
        setLF(0);
        setExp(0);
        return;
    }

    // Combat indices remain unchanged in both attack and defense
    float arvnLF = LF;
    float arvnEXP = EXP;

    int enemyLF = enemy->getLF();
    int enemyEXP = enemy->getExp();

    if (defense)
    {
        // // Defense logic: Liberation Army is attacking
        // auto combInfantry = enemy->findSmallestInfantryCombGreaterThan(arvnEXP);
        // auto combVehicle = enemy->findSmallestVehicleCombGreaterThan(arvnLF);
        // bool hasCombInfantry = !combInfantry.empty();
        // bool hasCombVehicle = !combVehicle.empty();
        // if (hasCombInfantry && hasCombVehicle)
        // {
        //     // Liberation Army wins: confiscate all ARVN units
        //     clearAllUnits();
        //     // Reduce weight of any remaining units by 20%
        //     this->reduceAllUnitsWeight(20);
        //     recalcIndices();
        // }
        // else if (hasCombInfantry && enemyLF > arvnLF)
        // {
        //     // Liberation Army wins: confiscate all ARVN units
        //     clearAllUnits();
        //     // Reduce weight of any remaining units by 20%
        //     this->reduceAllUnitsWeight(20);
        //     recalcIndices();
        // }
        // else if (hasCombVehicle && enemyEXP > arvnEXP)
        // {
        //     // Liberation Army wins: confiscate all ARVN units
        //     clearAllUnits();
        //     // Reduce weight of any remaining units by 20%
        //     this->reduceAllUnitsWeight(20);
        //     recalcIndices();
        // }
        // else
        // {
        //     // No battle occurs: ARVN forces remain intact
        //     // No changes to units or indices
        // }
    }
    else
    {
        // Attack logic: ARVN loses, reduce unit quantities by 20%
        Node *current = unitList->getHead();
        Node *prev = nullptr;
        while (current != nullptr)
        {
            Unit *unit = current->unit;
            int quantity = unit->getQuantity();
            unit->setQuantity(ceil(quantity * 0.8f)); // Reduce quantity by 20%

            if (unit->getQuantity() <= 1)
            {
                Node *toDelete = current;
                if (!prev)
                {
                    unitList->setHead(current->next);
                    current = current->next;
                }
                else
                {
                    prev->next = current->next;
                    current = current->next;
                }
                delete toDelete;
                continue;
            }
            prev = current;
            current = current->next;
        }
        // recalcIndices();
    }
}

string ARVN::str() const
{
    stringstream ss;
    ss << "ARVN["
       << "LF=" << LF
       << ",EXP=" << EXP
       << ",unitList=" << (unitList ? unitList->str() : "null")
       << ",battleField=" + ((this->battleField != nullptr) ? this->battleField->str() : "]");
    return ss.str();
}

ARVN::~ARVN() {}
// ARVN END HERE

// UnitList START HERE
bool UnitList::isSpecialNumber(int n)
{
    for (int k : {3, 5, 7})
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
    }

    return false;
}

UnitList::UnitList(int armyLF, int armyEXP) : head(nullptr), tail(nullptr), size(0)
{
    int S = armyLF + armyEXP;
    bool isSpecial = UnitList::isSpecialNumber(S);
    this->capacity = isSpecial ? 12 : 8;
    this->head = nullptr;
}

Node *UnitList::getHead()
{
    return head;
}

void UnitList::setHead(Node *newHead)
{
    this->head = newHead;
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
    // Validate input and capacity
    if (!unit || size >= capacity)
    {
        delete unit;
        return false;
    }
    Node *current = head;
    bool isInfantryType = unit->className() == "Infantry";
    while (current != nullptr)
    {
        bool sameType = false;
        stringstream stringType;
        stringType << unit->getStringType();
        stringstream currentstringType;
        currentstringType << current->unit->getStringType();

        if (unit->className() == current->unit->className())
        {
            if (currentstringType.str() == stringType.str())
            {
                current->unit->setQuantity(current->unit->getQuantity() + unit->getQuantity());
                delete unit;
                size++;
                return true;
            }
        }
        current = current->next;
    }

    // Create new node for insertion
    Node *newNode = new Node(unit);
    if (!newNode)
    {
        delete unit; // Free input unit on allocation failure
        return false;
    }

    // Insert infantry at head, vehicle at tail
    if (isInfantryType)
    {
        // Infantry: Insert at head
        newNode->next = head;
        head = newNode;
        if (!tail)
        {
            tail = newNode;
        }
    }
    else
    {
        // Vehicle: Insert at tail
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

    this->size++;
    return true;
}
bool UnitList::isExit(Unit *unit)
{
    Node *temp = head;
    while (temp != nullptr)
    {
        if (temp->unit == unit)
            return true;
        temp = temp->next;
    }
    return false;
}

void UnitList ::setCapacity(int x)
{
    this->capacity = x;
}
int UnitList ::getCapacity() const
{
    return this->capacity;
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
       << ";count_infantry=" << infantryCount << (infantryCount == 0 ? "" : ";");

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
        if (current->unit)
            result.push_back(current->unit);
        current = current->next;
    }
    return result;
}

void UnitList::removeUnit(Unit *unit)
{
    if (!head)
        return;
    while (head->next && head->next->unit != unit)
    {
        head = head->next;
    }
    if (!head->next)
        return;
    Node *xoa = head->next;
    head->next = head->next->next;
    delete xoa;
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

void UnitList::clear()
{
    Node *current = head;
    while (current)
    {
        Node *next = current->next;
        delete current->unit; // Delete the Unit object
        delete current;       // Delete the Node
        current = next;
    }
    head = tail = nullptr;
    size = 0;
}

UnitList::~UnitList()
{
    clear();
}
// UnitList END HERE

// Position START HERE
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
// Position END HERE

// TerrainElement START HERE
TerrainElement::~TerrainElement() {}

Road::Road() {}

void Road::getEffect(Army *army)
{
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
        if (pos.euclideanDistance(unit->getCurrentPosition()) <= radius)
        {
            if (Infantry *infantry = dynamic_cast<Infantry *>(unit))
            {
                deltaEXP += static_cast<int>(ceil(infantry->getAttackScore() * expMultiplier));
            }
            else if (Vehicle *vehicle = dynamic_cast<Vehicle *>(unit))
            {
                deltaLF += static_cast<int>(ceil(vehicle->getAttackScore() * lfMultiplier));
            }
        }
    }

    int newLF = army->getLF() + deltaLF;
    newLF = newLF < 0 ? 0 : newLF > 1000 ? 1000
                                         : newLF;
    int newEXP = army->getExp() + deltaEXP;
    newEXP = newEXP < 0 ? 0 : newEXP > 1000 ? 1000
                                            : newEXP;
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
        if (pos.euclideanDistance(unit->getCurrentPosition()) <= radius)
        {
            if (Infantry *infantry = dynamic_cast<Infantry *>(unit))
            {
                deltaEXP += static_cast<int>(ceil(infantry->getAttackScore() * multiplier));
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
            int distance = pos.euclideanDistance(unit->getCurrentPosition());
            if (Infantry *infantry = dynamic_cast<Infantry *>(unit))
            {
                string type = infantry->getStringType();
                if ((type == "SPECIALFORCES" || type == "REGULARINFANTRY") && distance <= 5)
                {
                    int D = distance == 0 ? 1 : distance;
                    deltaEXP += static_cast<int>(ceil(2.0 * infantry->getAttackScore() / D));
                }
            }
            else if (Vehicle *vehicle = dynamic_cast<Vehicle *>(unit))
            {
                if (vehicle->getStringType() == "ARTILLERY" && distance <= 2)
                {
                    deltaLF += static_cast<int>(ceil(vehicle->getAttackScore() * -0.5f));
                }
            }
        }
    }
    else if (dynamic_cast<ARVN *>(army))
    {
        for (Unit *unit : army->getUnitList()->getAllUnits())
        {
            int distance = pos.euclideanDistance(unit->getCurrentPosition());
            if (Infantry *infantry = dynamic_cast<Infantry *>(unit))
            {
                if (infantry->getStringType() == "REGULARINFANTRY" && distance <= 3)
                {
                    int D = distance == 0 ? 1 : distance;
                    deltaEXP += static_cast<int>(ceil(3.0 * infantry->getAttackScore() / (2.0 * D)));
                }
            }
        }
    }

    int newLF = army->getLF() + deltaLF;
    newLF = newLF < 0 ? 0 : newLF > 1000 ? 1000
                                         : newLF;
    int newEXP = army->getExp() + deltaEXP;
    newEXP = newEXP < 0 ? 0 : newEXP > 1000 ? 1000
                                            : newEXP;
    army->setLF(newLF);
    army->setExp(newEXP);
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

        if (pos.euclideanDistance(unit->getCurrentPosition()) <= radius)
        {
            if (Infantry *infantry = dynamic_cast<Infantry *>(unit))
            {
                deltaEXP += static_cast<int>(ceil(infantry->getAttackScore() * multiplier));
            }
            else if (Vehicle *vehicle = dynamic_cast<Vehicle *>(unit))
            {
                deltaLF += static_cast<int>(ceil(vehicle->getAttackScore() * multiplier));
            }
        }
    }
    int newLF = army->getLF() + deltaLF;
    newLF = newLF < 0 ? 0 : newLF > 1000 ? 1000
                                         : newLF;
    int newEXP = army->getExp() + deltaEXP;
    newEXP = newEXP < 0 ? 0 : newEXP > 1000 ? 1000
                                            : newEXP;
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
        if (pos.euclideanDistance(unit->getCurrentPosition()) <= radius)
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
    newLF = newLF < 0 ? 0 : newLF > 1000 ? 1000
                                         : newLF;
    int newEXP = army->getExp() + deltaEXP;
    newEXP = newEXP < 0 ? 0 : newEXP > 1000 ? 1000
                                            : newEXP;
    army->setLF(newLF);
    army->setExp(newEXP);
}
// TerrainElement END HERE

// BattleField START HERE
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

const vector<vector<TerrainElement *>> &BattleField::getTerrain() const
{
    return terrain;
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
// BattleField START HERE

// Configuration END HERE
void Configuration::parseStringToPosition(const string &key, string val, vector<Position *> &target)
{
    if (key != "ARRAY_FOREST" && key != "ARRAY_RIVER" &&
            key != "ARRAY_FORTIFICATION" && key != "ARRAY_URBAN" &&
            key != "ARRAY_SPECIAL_ZONE" ||
        val.empty())
    {
        return;
    }

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
            try
            {
                int row = stoi(pair.substr(0, comma));
                int col = stoi(pair.substr(comma + 1));
                target.push_back(new Position(row, col));
            }
            catch (...)
            {
            }
        }
        start = right + 1;
        while (start < val.size() && (val[start] == ',' || isspace(val[start])))
        {
            ++start;
        }
    }
}

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
            try
            {
                num_rows = stoi(value);
            }
            catch (...)
            {
                num_rows = 0;
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
                num_cols = 0;
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
                eventCode = 0;
            }
        }
        else if (key == "ARRAY_FOREST")
        {
            arrayForest.clear();
            this->parseStringToPosition(key, value, arrayForest);
        }
        else if (key == "ARRAY_RIVER")
        {
            arrayRiver.clear();
            this->parseStringToPosition(key, value, arrayRiver);
        }
        else if (key == "ARRAY_FORTIFICATION")
        {
            arrayFortification.clear();
            this->parseStringToPosition(key, value, arrayFortification);
        }
        else if (key == "ARRAY_URBAN")
        {
            arrayUrban.clear();
            this->parseStringToPosition(key, value, arrayUrban);
        }
        else if (key == "ARRAY_SPECIAL_ZONE")
        {
            arraySpecialZone.clear();
            this->parseStringToPosition(key, value, arraySpecialZone);
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
                bool isLebsArmy = false;
                Unit *unit = parseUnit(unitStr, isLebsArmy);
                if (unit)
                {
                    if (isLebsArmy)
                    {
                        liberationUnits.push_back(unit);
                    }
                    else
                    {
                        ARVNUnits.push_back(unit);
                    }
                }
            }
        }
    }
    file.close();
}

string Configuration::str() const
{
    stringstream ss;
    ss << "[";
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
    for (const Unit *unit : ARVNUnits)
    {
        if (unit)
        {
            if (!firstARVN)
                ss << ",";
            ss << unit->str();
            firstARVN = false;
        }
    }
    ss << "],";

    ss << "eventCode=" << eventCode << "]";
    return ss.str();
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

    liberationUnits.clear();
    ARVNUnits.clear();
}

// Configuration END HERE
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
    vector<Unit *> liberationUnits = config->getLiberationUnits();
    vector<Unit *> ARVNUnits = config->getARVNUnits();

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
    arvnArmy->recalcIndices();
    liberationArmy->recalcIndices();
    int eventCode = config->getEventCode();
    if (eventCode < 75)
    {
        liberationArmy->fight(arvnArmy, false);
        arvnArmy->fight(liberationArmy, true);
        arvnArmy->recalcIndices();
        liberationArmy->recalcIndices();
    }
    else
    {
        arvnArmy->fight(liberationArmy, false);
        liberationArmy->fight(arvnArmy, true);

        arvnArmy->recalcIndices();
        liberationArmy->recalcIndices();

        liberationArmy->fight(arvnArmy, true);
        arvnArmy->recalcIndices();
        liberationArmy->recalcIndices();
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

    arvnArmy->recalcIndices();
    liberationArmy->recalcIndices();
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