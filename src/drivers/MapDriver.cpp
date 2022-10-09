//
// COMP345_PROJECT_MAPDRIVER_CPP MapDriver.cpp
//

#include <iostream>
    using std::cout;
    using std::endl;

#include "../../include/Map.h"

void testLoadMaps()
{
    cout << endl
        << "------------------------------------------------------" << endl
        << "Test Load Maps" << endl
        << "------------------------------------------------------" << endl
        << endl;

    Map* map = MapLoader::load("./001_I72_Ghtroc720.map");
    delete map;
    cout << endl;
    map = MapLoader::load("./004_I72_Cobra.map");
    delete map;
    cout << endl;
    map = MapLoader::load("./Antarctica.map");
    delete map;
    cout << endl;
    map = MapLoader::load("./APKomp.map");
    delete map;
    cout << endl;
    map = MapLoader::load("./Cornwall.map");
    delete map;
    cout << endl;

    map = MapLoader::load("./duplicate_continent.map");
    delete map;
    cout << endl;
    map = MapLoader::load("./duplicate_territory.map");
    delete map;
    cout << endl;
    map = MapLoader::load("./missing_continent.map");
    delete map;
    cout << endl;
    map = MapLoader::load("./missing_territory.map");
    delete map;
    cout << endl;


}