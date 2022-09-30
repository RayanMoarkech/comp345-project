//
//
//

#include <string>
    using std::string;
#include <fstream>
    using std::ifstream;
#include <sstream>
    using std::stringstream;
#include <vector>
    using std::vector;
#include <map>

#include "../../include/MapLoader.h"
#include "../../include/Map.h"

MapLoader::MapLoader() = default;

void MapLoader::load(const std::string& mapFileDir)
{
    // Open a read stream to the file mapFileDir
    ifstream input(mapFileDir);
    string line;

    // Create a new Map
    Map* map = new Map();

    // Loop through the lines of the map config
    while (getline(input, line))
    {
        // Continent section
        if (line == "[Continents]\r")
        {
            // Loop through the lines
            while(getline(input, line))
            {
                if (line == "\r")
                {
                    break;
                }
                // Get the name and score of Continent
                unsigned long delimiterPos = line.find('=');
                string name = line.substr(0, delimiterPos);
                int score = std::stoi(line.substr(delimiterPos + 1));
                // Create a new Continent object
                Continent continent = Continent(name, score);
                // Add the Continent Object reference to the Map
                map->addContinent(&continent);
            }
        }
        // Territories section
        else if (line == "[Territories]\r")
        {
            // Create a map that stores the Territory object reference with the names of the adjacent Territories
            std::map<Territory*, vector<string>> territoryAdjMap;

            // Loop through each line
            while(getline(input, line))
            {
                // Use delimiter ',' to extract the Territory name from the line
                unsigned long delimiterPos = line.find(',');
                string name = line.substr(0, delimiterPos);

                // Use delimiter ',' to extract the Territory coordinates from the line
                line = line.substr(delimiterPos + 1);
                delimiterPos = line.find(',');
                int coordinateX = stoi(line.substr(0, delimiterPos));
                line = line.substr(delimiterPos + 1);
                delimiterPos = line.find(',');
                int coordinateY = stoi(line.substr(0, delimiterPos));

                // Use delimiter ',' to extract the Territory Continent from the line
                line = line.substr(delimiterPos + 1);
                delimiterPos = line.find(',');
                string continentName = line.substr(0, delimiterPos);
                // Get the Continent reference
                Continent* continent = map->getContinent(continentName);

                line = line.substr(delimiterPos + 1);

                // Create the Territory object
                Territory territory = Territory(name, coordinateX, coordinateY, continent);
                territoryAdjMap.insert(std::pair<Territory*, vector<string>>(&territory, {}));

                // Loop through the remaining line with a delimiter ',' to get the names of the adjacent territories
                string adjacentTerritoryName;
                stringstream lineStream;
                lineStream << line;
                while(getline(lineStream, adjacentTerritoryName, ','))
                {
                    territoryAdjMap[&territory].push_back(adjacentTerritoryName);
                }

            }

            for (auto const& pair: territoryAdjMap)
            {
                for(auto const& adjTerritoryName: pair.second)
                {
                    Territory* adjTerritory = map->getTerritory(adjTerritoryName);
                    pair.first->addAdjacentTerritory(adjTerritory);
                }
            }

        }
    }
    input.close();
}