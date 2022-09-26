//
// COMP345_PROJECT_MAP_H
//

#ifndef Map_H
#define Map_H

#include <iostream>

#include "../Continent/Continent.h"

class Map
{
private:
    std::vector<Continent*> continents;

public:
    Map();
    bool validate();
    // The Map class includes a validate() method that makes the following checks:
    // 1) the map is a connected graph,
    // 2) continents are connected subgraphs and
    // 3) each country belongs to one and only one continent.
};

#endif //Map_H
