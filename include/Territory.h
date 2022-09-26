//
// COMP345_PROJECT_TERRITORY_H
//

#ifndef TERRITORY_H
#define TERRITORY_H

#include <iostream>
#include <vector>

#include "Continent.h"

class Territory
{
private:
    int coordinateX;
    int coordinateY;
    std::string name;
    Continent* continent;
    std::vector<std::string> adjacentTerritoryNames; // The maximum allowed is 10. Maybe change to array?
//    Player* ownedBy; // TODO: A territory is owned by a player and contain a number of armies.
    int numberOfArmies;

public:
    Territory(std::string name, int coordinateX, int coordinateY, Continent* continent, std::vector<std::string> adjacentTerritoryNames);
};

#endif //TERRITORY_H
