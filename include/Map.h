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
    void addContinent(Continent* continent);
    Continent* getContinent(const std::string& name);
    bool validate();
};

#endif //Map_H
