//
//
//

#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include "../../include/MapLoader.h"
#include "../../include/Map.h"
#include "../../include/Territory.h"

MapLoader::MapLoader()
{};

void MapLoader::load(std::string mapFileDir)
{
    std::ifstream input(mapFileDir);
    std::string line;

    Map* map = new Map();

    while (getline(input, line))
    {
        if (line == "[Continents]\r")
        {
            while(getline(input, line))
            {
                unsigned long delimiterPos = line.find('=');
                std::string name = line.substr(0, delimiterPos);
                int score = std::stoi(line.substr(delimiterPos + 1));
                Continent continent = Continent(name, score);
                map->addContinent(&continent);
            }
        }
        else if (line == "[Territories]\r")
        {
            while(getline(input, line))
            {
                unsigned long delimiterPos = line.find(',');
                std::string name = line.substr(0, delimiterPos);

                line = line.substr(delimiterPos + 1);
                delimiterPos = line.find(',');
                int coordinateX = stoi(line.substr(0, delimiterPos));
                line = line.substr(delimiterPos + 1);
                delimiterPos = line.find(',');
                int coordinateY = stoi(line.substr(0, delimiterPos));

                line = line.substr(delimiterPos + 1);
                delimiterPos = line.find(',');
                std::string continentName = line.substr(0, delimiterPos);
                Continent* continent = map->getContinent(continentName);

                std::vector<std::string> adjacentTerritoryNames;
                std::string adjacentTerritoryName;
                std::stringstream lineStream;
                lineStream << line;
                while(getline(lineStream, adjacentTerritoryName, ','))
                {
                    adjacentTerritoryNames.push_back(adjacentTerritoryName);
                }

                Territory territory = Territory(name, coordinateX, coordinateY, continent, adjacentTerritoryNames);
            }
        }
    }
    input.close();
}