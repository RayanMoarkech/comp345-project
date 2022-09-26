#include <iostream>

#include "../../include/MapLoader.h"

int testLoadMaps()
{
    MapLoader mapLoader = MapLoader();
    mapLoader.load("../../config/001_I72_Ghtroc 720.MapLoader");
    return 0;
}
