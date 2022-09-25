//
// COMP345_PROJECT_MAPLOADER_H
//

#ifndef MapLoader_H
#define MapLoader_H

#include <string>

class MapLoader
{
    private:
        std::string mapFileDir;

    public:
        MapLoader();
        void load(std::string mapFileDir);

};

#endif //MapLoader_H
