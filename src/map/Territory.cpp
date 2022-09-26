//
//
//

#include <utility>

#include "../../include/Territory.h"

Territory::Territory(std::string name, int coordinateX, int coordinateY, Continent* continent,
                     std::vector<std::string> adjacentTerritoryNames)
{
    this->name = std::move(name);
    this->coordinateX = coordinateX;
    this->coordinateY = coordinateY;
    this->continent = continent;
    this->adjacentTerritoryNames = std::move(adjacentTerritoryNames);
    this->numberOfArmies = 0;
}