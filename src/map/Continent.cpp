//
//
//

#include <utility>
#include <vector>

#include "../../include/Continent.h"

Continent::Continent(std::string name, int score)
{
    this->name = std::move(name);
    this->score = score;
    this->territories = {};
}

std::string Continent::getName()
{
    return name;
}

int Continent::getScore() const
{
    return score;
}

std::vector<Territory*> Continent::getTerritories()
{
    return territories;
}