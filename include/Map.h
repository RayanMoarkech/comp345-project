//
// COMP345_PROJECT_MAP_H
//

#ifndef Map_H
#define Map_H

#include <iostream>

#include "Continent.h"

class Map
{
private:
    std::vector<Continent*> continents;

public:
    Map();
    bool validate();
};

#endif //Map_H
