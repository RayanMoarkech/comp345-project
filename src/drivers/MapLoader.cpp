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
#include <iostream>
    using std::cout;
    using std::endl;

#include "../../include/MapLoader.h"
#include "../../include/Map.h"

MapLoader::MapLoader() = default;

void MapLoader::load(const std::string& mapFileDir)
{
    // Create a new Map
    Map* map = new Map();

    try {
        // Open a read stream to the file mapFileDir
        ifstream input(mapFileDir);
        string line;

        // Loop through the lines of the map config
        while (getline(input, line)) {
            // Continent section
            if (line == "[Continents]\r") {
                // Loop through the lines
                while (getline(input, line)) {
                    if (line == "\r") {
                        break;
                    }
                    // Get the name and score of Continent
                    unsigned long delimiterPos = line.find('=');
                    string name = line.substr(0, delimiterPos);
                    int score = std::stoi(line.substr(delimiterPos + 1));
                    // Create a new Continent object
                    Continent* continent = new Continent(name, score);
                    // Add the Continent Object reference to the Map
                    map->addContinent(continent);
                }
            }
                // Territories section
            else if (line == "[Territories]\r") {
                // Create a map that stores the Territory object reference with the names of the adjacent Territories
                std::map<Territory*, vector<string>> territoryAdjMap;

                // Loop through each line
                while (getline(input, line)) {
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
                    Territory* territory = new Territory(name, coordinateX, coordinateY, continent);
                    territoryAdjMap.insert(std::pair<Territory *, vector<string>>(territory, {}));

                    // Add the territory to the map and corresponding continent
                    map->addTerritory(territory);
                    continent->addTerritory(territory);

                    // Loop through the remaining line with a delimiter ',' to get the names of the adjacent territories
                    string adjacentTerritoryName;
                    stringstream lineStream;
                    lineStream << line;
                    while (getline(lineStream, adjacentTerritoryName, ',')) {
                        if (adjacentTerritoryName.find('\r') != std::string::npos) {
                            adjacentTerritoryName.pop_back();
                        }
                        territoryAdjMap[territory].push_back(adjacentTerritoryName);
                    }

                }

                for (auto const &pair: territoryAdjMap)
                {
                    for (auto const &adjTerritoryName: pair.second)
                    {
                        Territory *adjTerritory = map->getTerritory(adjTerritoryName);
                        // if adjTerritory is a nullptr
                        if (!adjTerritory)
                        {
                            cout << pair.first->getName() << endl;
                            cout << adjTerritoryName << endl;
                            // Could not find territory
                            throw std::exception();
                        }
                        pair.first->addAdjacentTerritory(adjTerritory);
                    }
                }

            }
        }
        input.close();
    } catch (...) {
        cout << "An error has occurred when creating the map from config file: " << mapFileDir << std::endl;
        map->setValidFalse();
    }
    cout << std::boolalpha << mapFileDir << " is valid: " << map->validate() << endl;
}