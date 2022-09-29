//
//
//

#include <utility>
#include <string>
    using std::string;

#include "../../include/Territory.h"

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