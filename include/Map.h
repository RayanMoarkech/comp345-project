//
// COMP345_PROJECT_MAP_H Map.h
//

#ifndef Map_H
#define Map_H

#include <string>
    using std::string;
#include <vector>
    using std::vector;
#include <ostream>
    using std::ostream;

class Continent; // Used to determine size of the pointer
class Territory; // Used to determine size of the pointer
class Player;


// ---------------------------------------------
// ---------------- Map Section ----------------
// ---------------------------------------------

class Map
{
private:
    vector<Continent*> continents;
    vector<Territory*> territories;
    bool isValid;

public:
    Map();
    Map(const Map& map);
    ~Map();
    void addContinent(Continent* continent);
    Continent* getContinent(const string& name);
    void addTerritory(Territory* territory);
    Territory* getTerritory(const string& name);
    vector<Territory*> getTerritories();
    void setValidFalse();
    bool validate();
    friend ostream& operator<<(ostream& os, Map& map);
    Map& operator=(const Map& map);
};


// ---------------------------------------------
// ------------- Continent Section -------------
// ---------------------------------------------

class Continent
{
private:
    string name;
    int score;
//    vector<Territory*> territories;

public:
    Continent(string name, int score);
    Continent(const Continent& continent);
    ~Continent();
    string getName();
    int getScore() const;
//    void addTerritory(Territory* territory);
    friend ostream& operator<<(ostream& os, Continent& continent);
    Continent& operator=(const Continent& continent);
};


// ---------------------------------------------
// ------------- Territory Section -------------
// ---------------------------------------------

class Territory
{
private:
    int coordinateX;
    int coordinateY;
    string name;
    Continent* continent;
    vector<Territory*> adjacentTerritories; // The maximum allowed is 10. Maybe change to array?
    Player* ownedBy;
    int numberOfArmies;

public:
    Territory(string name, int coordinateX, int coordinateY, Continent* continent);
    Territory(const Territory& territory, Continent* continent = nullptr);
    ~Territory();
    string getName();
    Continent* getContinent();
    void addAdjacentTerritory(Territory* territory);
    vector<Territory*> getAdjacentTerritories();
    void setOwnedBy(Player* ownedBy, int numberOfArmies);
    friend ostream& operator<<(ostream& os, Territory& territory);
    Territory& operator=(const Territory& territory);
};


// ---------------------------------------------
// ------------- MapLoader Section -------------
// ---------------------------------------------

class MapLoader
{
private:

public:
    MapLoader();
    MapLoader(const MapLoader& mapLoader);
    ~MapLoader();
    static Map* load(const string& mapFileDir);
    friend ostream& operator<<(ostream& os, MapLoader& mapLoader);
    MapLoader& operator=(const MapLoader& mapLoader);
};


#endif
