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
class TerrainElement;

class Road;
class Mountain;
class River;
class Urban;
class Fortification;
class SpecialZone;

class BattleField;

class HCMCampaign;
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
    UnitList *getUnitList() const { return unitList; }
    int getLF();
    void setLF(int lf);
    int getExp();
    void setExp(int exp);
};
class LiberationArmy : public Army
{
public:
    LiberationArmy(const Unit **unitArray, int size, string name,
                   BattleField *battleField);
    virtual void fight(Army *enemy, bool defence = false) override;
    virtual string str() override;
    // Xoá đơn vị
    void removeUnits(Unit unit);
    // Tìm tổ hợp bộ binh nhỏ nhất có sức mạnh lớn hơn
    vector<Unit *> findSmallestInfantryCombGreaterThan(int enemyExp);
    // Tìm tổ hợp phương tiện nhỏ nhất có sức mạnh lớn hơn
    vector<Unit *> findSmallestVehicleCombGreaterThan(int enemyLF);
    // Tịch thu chiến lợi phẩm
    void confiscateEnemyUnits(Army *enemy);
    // Loại bỏ toàn bộ bộ binh
    void eliminateAllInfantry();
    // Loại bỏ toàn bộ phương tiện chiến đấu
    void eliminateAllVehicles();
    // Tính toán lại sức mạnh quân đội
    void recalcIndices();
    // Giảm toàn bộ sức mạnh quân đội
    void reduceAllUnitsWeight(int num);
    // Tăng sức mạnh quân đội lên số Fibonaci gần nhất
    void reinforceUnitsWithFibonacci();

    virtual ~LiberationArmy();
};
class ARVN : public Army
{
public:
    ARVN(const Unit **unitArray, int size, string name, BattleField *battleField);
    void fight(Army *enemy, bool defense);
    void ~ARVN();
    string str() const;
} class Position
{
private:
    int r, c;

public:
    Position(int r = 0, int c = 0);
    Position(const string &str_pos); // Example: str_pos = "(1,15)"
    int getRow() const;
    int getCol() const;
    void setRow(int r);
    void setCol(int c);
    string str() const; // Example: returns "(1,15)"
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
    Vehicle(VehicleType vehicleType, int quantity, int weight, const Position pos);
    ~Vehicle();
    int getAttackScore() override;
    string str() const override;
    string getStringType() const;
    VehicleType getVehicleType() const;
};
class Infantry : public Unit
{
private:
    InfantryType infantryType;
    bool isPerfectSquare(int n) const {};
    int personalNumber(int num, int year = 1975) const {};
    int digitSum(int n) const {};

public:
    Infantry(int quantity, int weight, const Position pos, InfantryType infantryType);
    ~Infantry();
    int getAttackScore() const override;
    string str() const override;
    string getStringType() const;

}

class UnitList
{
private:
    Unit **units;
    int capacity;
    int size;

public:
    UnitList(int capacity);
    bool insert(Unit *unit);                   // return true if insert successfully
    bool isContain(VehicleType vehicleType);   // return true if it exists
    bool isContain(InfantryType infantryType); // return true if it exists
    string str() const;
    vector<Unit *> getAllUnits() const;
    void removeUnit(Unit *unit) const
        void clear();
    ~UnitList();

private:
    string vehicleTypeToString(Vehicle type) const
        string infantryTypeToString(InfantryType type) const
};

class TerrainElement
{
public:
    TerrainElement();
    ~TerrainElement();
    virtual void getEffect(Army *army) = 0;
};

class BattleField
{
private:
    int n_rows, n_cols;
    // TODO
public:
    BattleField(int n_rows, int n_cols, vector<Position *> arrayForest,
                vector<Position *> arrayRiver, vector<Position *> arrayFortification,
                vector<Position *> arrayUrban, vector<Position *> arraySpecialZone);
    ~BattleField();
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
};

#endif