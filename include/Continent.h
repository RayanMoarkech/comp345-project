//
// COMP345_PROJECT_CONTINENT_H
//

#ifndef CONTINENT_H
#define CONTINENT_H

#include <string>
    using std::string;
#include <vector>
    using std::vector;

#include "Territory.h"

class Territory; // Used to determine size of the pointer

class Continent
{
private:
    string name;
    int score;
    vector<Territory*> territories;
    // Maybe add later pointer to map object.

public:
    Continent(string name, int score);
    string getName();
    int getScore() const;
    vector<Territory*> getTerritories();
};

#endif //CONTINENT_H
