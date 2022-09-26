//
//
//

#include "../../include/Territory.h"

Territory::Territory(std::string name, int coordinateX, int coordinateY, std::string continentName,
                     std::vector<Territory *> adjacentTerritories)
{
    this->name = name;
    this->coordinateX = coordinateX;
    this->coordinateY = coordinateY;
    this->continentName = continentName;
    this->adjacentTerritories = adjacentTerritories;
}