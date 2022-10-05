//
// Map.cpp
//

#include <string>
    using std::string;
#include <vector>
    using std::vector;

#include "../../include/Map.h"


// ---------------------------------------------
// ---------------- Map Section ----------------
// ---------------------------------------------

Map::Map()
{
    this->continents = {};
    this->isValid = true;
}

Map::Map(const Map& map)
{
    this->continents = {};
    for (Continent* continent: map.continents)
    {
        continents.push_back(new Continent(*continent));
    }
    this->territories = {};
    for (Territory* territory: map.territories)
    {
        this->territories.push_back(new Territory(*territory));
    }
    this->isValid = map.isValid;
}

Map::~Map()
{
    for (Continent* continent: this->continents)
    {
        delete continent;
        continent = nullptr;
    }
    for (Territory* territory: this->territories)
    {
        delete territory;
        territory = nullptr;
    }
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

void Map::setValidFalse()
{
    this->isValid = false;
}

// Method that makes the following checks:
// 1) the map is a connected graph,
// 2) continents are connected sub-graphs, and
// 3) each country belongs to one and only one continent.
bool Map::validate()
{
    // The load function already checks if there is any adjacent territory that is not a territory
    // and if the territory is in a continent that does not exist.
    // If the map is not valid, skip the check.
    if (!this->isValid)
    {
        return false;
    }

    // Check if there is a duplicate Continent with the same name
    vector<std::string> continentNames = {};
    for (Continent* continent: this->continents)
    {
        const string continentName = continent->getName();
        if (std::find(continentNames.begin(), continentNames.end(), continentName) != continentNames.end())
        {
            this->isValid = false;
            return false;
        }
        continentNames.push_back(continentName);
    }

    // Check if there is a duplicate Territory with the same name
    vector<std::string> territoryNames = {};
    for (Territory* territory: this->territories)
    {
        const string territoryName = territory->getName();
        if (std::find(territoryNames.begin(), territoryNames.end(), territoryName) != territoryNames.end())
        {
            this->isValid = false;
            return false;
        }
        continentNames.push_back(territoryName);
    }

    return true;
}


// ---------------------------------------------
// ------------- Continent Section -------------
// ---------------------------------------------

Continent::Continent(std::string name, int score)
{
    this->name = std::move(name);
    this->score = score;
    this->territories = {};
}

Continent::Continent(const Continent& continent)
{
    this->name = continent.name;
    this->score = continent.score;
    this->territories = {};
    for (Territory* territory: continent.territories)
    {
        this->territories.push_back(new Territory(*territory));
    }
}

Continent::~Continent() = default;

std::string Continent::getName()
{
    return name;
}

int Continent::getScore() const
{
    return score;
}

void Continent::addTerritory(Territory* territory)
{
    this->territories.push_back(territory);
}


// ---------------------------------------------
// ------------- Territory Section -------------
// ---------------------------------------------

Territory::Territory(string name, int coordinateX, int coordinateY, Continent* continent)
{
    this->name = std::move(name);
    this->coordinateX = coordinateX;
    this->coordinateY = coordinateY;
    this->continent = continent;
    this->adjacentTerritories = {};
    this->numberOfArmies = 0;
}

Territory::Territory(const Territory& territory)
{
    this->coordinateX = territory.coordinateX;
    this->coordinateY = territory.coordinateY;
    this->name = territory.name;
    this->continent = new Continent(*territory.continent);
    this->adjacentTerritories = {};
    for (Territory* adjTerritory: territory.adjacentTerritories)
    {
        this->adjacentTerritories.push_back(new Territory(*adjTerritory));
    }
    // TODO
//    this->ownedBy = territory.ownedBy;
    this->numberOfArmies = territory.numberOfArmies;
}

Territory::~Territory() = default;

string Territory::getName()
{
    return this->name;
}

void Territory::addAdjacentTerritory(Territory *territory)
{
    this->adjacentTerritories.push_back(territory);
}