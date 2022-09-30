//
// Map.cpp
//

#include <string>
    using std::string;
#include <vector>
    using std::vector;

#include "../../include/Map.h"


//
// Map Section
//

Map::Map()
{
    this->continents = {};
}

void Map::addContinent(Continent* continent)
{
    this->continents.push_back(continent);
}

Continent* Map::getContinent(const std::string& name)
{
    for(auto& continent: this->continents)
    {
        if (continent->getName() == name)
        {
            return continent;
        }
    }
    return nullptr;
}

void Map::addTerritory(Territory* territory)
{
    this->territories.push_back(territory);
}

Territory* Map::getTerritory(const std::string &name)
{
    for(auto& territory: this->territories)
    {
        if (territory->getName() == name)
        {
            return territory;
        }
    }
    return nullptr;
}

// The Map class includes a validate() method that makes the following checks:
// 1) the map is a connected graph,
// 2) continents are connected subgraphs and
// 3) each country belongs to one and only one continent.
bool Map::validate()
{
    bool isValid = false;
    //TODO: the checks
    return isValid;
}


//
// Continent Section
//

Continent::Continent(std::string name, int score)
{
    this->name = std::move(name);
    this->score = score;
    this->territories = {};
}

std::string Continent::getName()
{
    return name;
}

int Continent::getScore() const
{
    return score;
}

std::vector<Territory*> Continent::getTerritories()
{
    return territories;
}


//
// Territory Section
//

Territory::Territory(string name, int coordinateX, int coordinateY, Continent* continent)
{
    this->name = std::move(name);
    this->coordinateX = coordinateX;
    this->coordinateY = coordinateY;
    this->continent = continent;
    this->adjacentTerritories = {};
    this->numberOfArmies = 0;
}

string Territory::getName()
{
    return this->name;
}

void Territory::addAdjacentTerritory(Territory *territory)
{
    this->adjacentTerritories.push_back(territory);
}