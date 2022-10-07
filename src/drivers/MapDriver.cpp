//
// COMP345_PROJECT_MAPDRIVER_CPP MapDriver.cpp
//

#include "../../include/Map.h"

void testLoadMaps()
{
    MapLoader::load("./001_I72_Ghtroc720.map");
    MapLoader::load("./004_I72_Cobra.map");
    MapLoader::load("./Antarctica.map");
    MapLoader::load("./Cornwall.map");
}