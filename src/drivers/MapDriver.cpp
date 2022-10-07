//
// COMP345_PROJECT_MAPDRIVER_CPP MapDriver.cpp
//

#include "../../include/Map.h"

void testLoadMaps()
{
    Map* map = MapLoader::load("./001_I72_Ghtroc720.map");
    delete map;
    map = MapLoader::load("./004_I72_Cobra.map");
    delete map;
    map = MapLoader::load("./Antarctica.map");
    delete map;
    map = MapLoader::load("./APKomp.map");
    delete map;
    map = MapLoader::load("./Cornwall.map");
    delete map;

    map = MapLoader::load("./duplicate_continent.map");
    delete map;
    map = MapLoader::load("./duplicate_territory.map");
    delete map;
    map = MapLoader::load("./missing_continent.map");
    delete map;
    map = MapLoader::load("./missing_territory.map");
    delete map;
}