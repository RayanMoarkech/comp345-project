//
// COMP345_PROJECT_MAP_H Map.h
//

#ifndef Map_H
#define Map_H

#include <string>
    using std::string;
#include <vector>
    using std::vector;

class Continent; // Used to determine size of the pointer
class Territory; // Used to determine size of the pointer


//
// Map Section
//

class Map
{
private:
    vector<Continent*> continents;
    vector<Territory*> territories;
    bool isValid;

public:
    Map();
    ~Map();
    void addContinent(Continent* continent);
    Continent* getContinent(const string& name);
    void addTerritory(Territory* territory);
    Territory* getTerritory(const string& name);
    void setValidFalse();
    bool validate();
};


//
// Continent Section
//

class Continent
{
private:
    string name;
    int score;
    vector<Territory*> territories;
    // Maybe add later pointer to map object.

public:
    Continent(string name, int score);
    ~Continent();
    string getName();
    int getScore() const;
    void addTerritory(Territory* territory);
};


//
// Territory Section
//

class Territory
{
private:
    int coordinateX;
    int coordinateY;
    string name;
    Continent* continent;
    vector<Territory*> adjacentTerritories; // The maximum allowed is 10. Maybe change to array?
//    Player* ownedBy; // TODO: A territory is owned by a player and contain a number of armies.
    int numberOfArmies;

public:
    Territory(string name, int coordinateX, int coordinateY, Continent* continent);
    ~Territory();
    string getName();
    void addAdjacentTerritory(Territory* territory);
};

#endif //Map_H
