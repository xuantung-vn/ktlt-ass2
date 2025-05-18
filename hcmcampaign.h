/*
 * Ho Chi Minh City University of Technology
 * Faculty of Computer Science and Engineering
 * Initial code for Assignment 2
 * Programming Fundamentals Spring 2025
 * Date: 02.02.2025
 */

// The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_HCM_CAMPAIGN_H_
#define _H_HCM_CAMPAIGN_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class HCMCampaign;
class Army;
class LiberationArmy;
class ARVN;
class Position;
class Unit;
class Vehicle;
class Infantry;
class UnitList;
class TerrainElement;
class Road;
class Mountain;
class River;
class Urban;
class Fortification;
class SpecialZone;
class BattleField;
class Configuration;

enum VehicleType
{
    TRUCK,
    MORTAR,
    ANTIAIRCRAFT,
    ARMOREDCAR,
    APC,
    ARTILLERY,
    TANK
};
enum InfantryType
{
    SNIPER,
    ANTIAIRCRAFTSQUAD,
    MORTARSQUAD,
    ENGINEER,
    SPECIALFORCES,
    REGULARINFANTRY
};

class Army
{
protected:
    int LF, EXP;
    string name;
    UnitList *unitList;
    BattleField *battleField;

public:
    Army(Unit **unitArray, int size, string name, BattleField *battleField);
    virtual void fight(Army *enemy, bool defense = false) = 0;
    virtual string str() const = 0;
    virtual ~Army();
    UnitList *getUnitList() const;
    int getLF();
    void setLF(int lf);
    int getExp();
    void setExp(int exp);
};

class LiberationArmy : public Army
{
public:
    LiberationArmy(Unit **unitArray, int size, string name, BattleField *battleField);
    virtual void fight(Army *enemy, bool defence = false) override;
    virtual string str() const override;
    void removeUnits(vector<Unit *> &units);
    void removeUnit(Unit *unit);
    void recalcIndices();
    void confiscateEnemyUnits(Army *enemy);
    vector<Unit *> findSmallestInfantryCombGreaterThan(int enemyEXP);
    vector<Unit *> findSmallestVehicleCombGreaterThan(int enemyLF);
    void eliminateAllInfantry();
    void eliminateAllVehicles();
    void reduceAllUnitsWeight(int percentage);
    void reinforceUnitsWithFibonacci();
    virtual ~LiberationArmy();
};

class ARVN : public Army
{
public:
    ARVN(Unit **unitArray, int size, string name, BattleField *battleField);
    virtual void fight(Army *enemy, bool defence = false) override;
    virtual string str() const override;
    virtual ~ARVN();
};

class Position
{
private:
    int r, c;

public:
    Position(int r = 0, int c = 0);
    Position(const string &str_pos);
    Position *parsePosition(const string &posStr);
    int getRow() const;
    int getCol() const;
    void setRow(int r);
    void setCol(int c);
    string str() const;
};

class Unit
{
protected:
    int quantity, weight;
    Position pos;

public:
    Unit(int quantity, int weight, Position pos);
    virtual ~Unit();
    virtual int getAttackScore() = 0;
    Position getCurrentPosition() const;
    virtual string str() const = 0;
    void setQuantity(int quan);
    void setWeight(int w);
    void setPos(Position pos);
    int getQuantity() const;
    int getWeight() const;
};

class Vehicle : public Unit
{
private:
    VehicleType vehicleType;

public:
    Vehicle(int quantity, int weight, const Position pos, VehicleType vehicleType);
    virtual ~Vehicle();
    int getAttackScore() override;
    string str() const override;
    string getStringType() const;
    VehicleType getVehicleType() const;
};

class Infantry : public Unit
{
private:
    InfantryType infantryType;
    bool isPerfectSquare(int n) const;
    int personalNumber(int num, int year = 1975) const;
    int digitSum(int n) const;

public:
    Infantry(int quantity, int weight, const Position pos, InfantryType infantryType);
    virtual ~Infantry();
    int getAttackScore() override;
    string str() const override;
    string getStringType() const;
};

class UnitList
{
private:
    struct Node
    {
        Unit *unit;
        Node *next;
        Node(Unit *u) : unit(u), next(nullptr) {}
    };
    Node *head;
    Node *tail;
    int capacity;
    int size;
    string vehicleTypeToString(VehicleType type) const;
    string infantryTypeToString(InfantryType type) const;

public:
    UnitList(int armyLF, int armyEXP);
    bool insert(Unit *unit);
    bool isContain(VehicleType vehicleType);
    bool isContain(InfantryType infantryType);
    string str() const;
    vector<Unit *> getAllUnits() const;
    void removeUnit(Unit *unit);
    void clear();
    ~UnitList();
};

class TerrainElement
{
public:
    TerrainElement();
    virtual ~TerrainElement();
    virtual void getEffect(Army *army) = 0;
};

class Road : public TerrainElement
{
public:
    Road();
    void getEffect(Army *army) override;
};

class Mountain : public TerrainElement
{
private:
    Position pos;

public:
    Mountain(const Position &pos);
    void getEffect(Army *army) override;
};

class River : public TerrainElement
{
private:
    Position pos;

public:
    River(const Position &pos);
    void getEffect(Army *army) override;
};

class Urban : public TerrainElement
{
private:
    Position pos;

public:
    Urban(const Position &pos);
    void getEffect(Army *army) override;
};

class Fortification : public TerrainElement
{
private:
    Position pos;

public:
    Fortification(const Position &pos);
    void getEffect(Army *army) override;
};

class SpecialZone : public TerrainElement
{
private:
    Position pos;

public:
    SpecialZone(const Position &pos);
    void getEffect(Army *army) override;
};

class BattleField
{
private:
    int n_rows, n_cols;
    TerrainElement **terrain;

public:
    BattleField(int n_rows, int n_cols, vector<Position *> arrayForest,
                vector<Position *> arrayRiver, vector<Position *> arrayFortification,
                vector<Position *> arrayUrban, vector<Position *> arraySpecialZone);
    string str() const;
    ~BattleField();
    TerrainElement **getTerrain() const { return terrain; }
    int getRows() const { return n_rows; }
    int getCols() const { return n_cols; }
};

class Configuration
{
private:
    int num_rows, num_cols;
    vector<Position *> arrayForest, arrayRiver, arrayFortification, arrayUrban, arraySpecialZone;
    Unit **liberationUnits;
    int liberationUnitsSize;
    Unit **ARVNUnits;
    int ARVNUnitsSize;
    int eventCode;

public:
    Configuration(const string &filepath);
    string str() const;
    ~Configuration();
    int getNumRows() const { return num_rows; }
    int getNumCols() const { return num_cols; }
    const vector<Position *> &getArrayForest() const { return arrayForest; }
    const vector<Position *> &getArrayRiver() const { return arrayRiver; }
    const vector<Position *> &getArrayFortification() const { return arrayFortification; }
    const vector<Position *> &getArrayUrban() const { return arrayUrban; }
    const vector<Position *> &getArraySpecialZone() const { return arraySpecialZone; }
    Unit **getLiberationUnits() const { return liberationUnits; }
    int getLiberationUnitsSize() const { return liberationUnitsSize; }
    Unit **getARVNUnits() const { return ARVNUnits; }
    int getARVNUnitsSize() const { return ARVNUnitsSize; }
    int getEventCode() const { return eventCode; }
};

class HCMCampaign
{
private:
    Configuration *config;
    BattleField *battleField;
    LiberationArmy *liberationArmy;
    ARVN *ARVN;

public:
    HCMCampaign(const string &config_file_path);
    void run();
    string printResult();
    ~HCMCampaign();
};

#endif