//
// COMP345_PROJECT_CONTINENT_H
//

#ifndef CONTINENT_H
#define CONTINENT_H

#include <vector>

#include "../Territory/Territory.h"

class Continent
{
private:
    std::string name;
    int score;
    std::vector<Territory*> territories;
    // Maybe add later pointer to map object.

public:
    Continent();
    Continent(std::string name, int score);
    std::string getName();
    int getScore();
    std::vector<Territory> getTerritories();
};

#endif //CONTINENT_H