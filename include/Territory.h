//
// COMP345_PROJECT_TERRITORY_H
//

#ifndef TERRITORY_H
#define TERRITORY_H

#include <string>
    using std::string;
#include <vector>
    using std::vector;

#include "Continent.h"

class Continent; // Used to determine size of the pointer

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
    string getName();
    void addAdjacentTerritory(Territory* territory);
};

#endif //TERRITORY_H
