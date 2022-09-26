//
// COMP345_PROJECT_TERRITORY_H
//

#ifndef TERRITORY_H
#define TERRITORY_H

#include <iostream>
#include <vector>

class Territory
{
private:
    int coordinateX;
    int coordinateY;
    std::string name;
    std::string continentName; // References the Continent. A territory can be in only one Continent. Maybe switch later to pointer of object.
    std::vector<Territory*> adjacentTerritories; // The maximum allowed is 10. Maybe change to array?
//    Player* ownedBy; // TODO: A territory is owned by a player and contain a number of armies.
    int numberOfArmies;

public:
    Territory(std::string name, int coordinateX, int coordinateY, std::string continentName, std::vector<Territory*> adjacentTerritories);
};

#endif //TERRITORY_H
