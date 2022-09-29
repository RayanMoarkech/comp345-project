//
// COMP345_PROJECT_MAP_H
//

#ifndef Map_H
#define Map_H

#include <string>
    using std::string;

#include "Continent.h"
#include "Territory.h"

class Continent; // Used to determine size of the pointer

class Map
{
private:
    std::vector<Continent*> continents;
    std::vector<Territory*> territories;

public:
    Map();
    void addContinent(Continent* continent);
    Continent* getContinent(const string& name);
    void addTerritory(Territory* territory);
    Territory* getTerritory(const string& name);
    bool validate();
};

#endif //Map_H
