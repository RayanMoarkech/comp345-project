//
//
//

#include "../../include/Map.h"

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