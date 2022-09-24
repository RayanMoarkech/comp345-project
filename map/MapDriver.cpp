#include <iostream>

#include "MapLoader.h"

int testLoadMaps()
{
    MapLoader mapLoader = MapLoader();
    mapLoader.load("./configs/001_I72_Ghtroc 720.map");
    return 0;
}
