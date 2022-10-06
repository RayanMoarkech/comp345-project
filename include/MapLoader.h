//
// COMP345_PROJECT_MAPLOADER_H
//

#ifndef MapLoader_H
#define MapLoader_H

#include <string>
#include "Map.h"

class MapLoader
{
    private:

    public:
        MapLoader();
        static Map* load(const std::string& mapFileDir);

};

#endif //MapLoader_H
