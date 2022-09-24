//
//
//

#include "MapLoader.h"
#include <string>
#include <fstream>

MapLoader::MapLoader()
{
    std::cout << "Hello";
};

void MapLoader::load(std::string mapFileDir)
{
    std::ifstream input(mapFileDir);
    while (!input.eof()) {

    }
    input.close();
}