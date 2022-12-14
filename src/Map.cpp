//
// COMP345_PROJECT_MAP_CPP Map.cpp
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
#include <algorithm>
    using std::for_each;

#include "../include/Map.h"
#include "../include/Player.h"


// ---------------------------------------------
// ---------------- Map Section ----------------
// ---------------------------------------------

Map::Map()
{
    this->continents = {};
    this->territories = {};
    this->isValid = true;
}

Map::Map(const Map& map)
{
    // Deep copy all continents
    this->continents = {};
    for (Continent* continent: map.continents)
    {
        continents.push_back(new Continent(*continent));
    }
    // Deep copy all territories
    this->territories = {};
    vector<Territory*> originalTerritories = map.territories;
    for (Territory* territory: originalTerritories)
    {
        // Get the new continent object of the territory
        Continent* newContinent = this->getContinent(territory->getContinent()->getName());
        // Create a new territory
        Territory* newTerritory = new Territory(*territory, newContinent);
        // Add this new territory to the list of territories of the map
        this->territories.push_back(newTerritory);
        // Get the new created territories created by the new territory for the adjacency graph of the territories
        vector<Territory*> newAdjTerritories = newTerritory->getAdjacentTerritories();
        // Insert these new created territories that are adjacent to the new territory into the map territories
        this->territories.insert(std::end(this->territories), std::begin(newAdjTerritories), std::end(newAdjTerritories));
        // Remove these adjacent territories that were created, to avoid looping through them again
        for (Territory* originalAdjTerritory: territory->getAdjacentTerritories())
        {
            remove(originalTerritories.begin(), originalTerritories.end(), originalAdjTerritory);
        }
    }
    // Copy isValid
    this->isValid = map.isValid;
}

Map::~Map()
{
    // Delete all Continents
    for (Continent* continent: this->continents)
    {
        delete continent;
        continent = nullptr;
    }
    // Delete all Territories
    for (Territory* territory: this->territories)
    {
        delete territory;
        territory = nullptr;
    }
}

void Map::addContinent(Continent* continent)
{
    this->continents.push_back(continent);
}

// Return nullptr if not found
Continent* Map::getContinent(const std::string& name)
{
    auto* continentName = new string(name);
    for_each((*continentName).begin(), (*continentName).end(), [](char & c) {
        c = ::tolower(c);
    });
    for(auto& continent: this->continents)
    {
        if (continent->getName() == *continentName)
        {
            return continent;
        }
    }
    return nullptr;
}

void Map::addTerritory(Territory* territory)
{
    this->territories.push_back(territory);
}

// Return nullptr if not found
Territory* Map::getTerritory(const std::string &name)
{
    auto* territoryName = new string(name);
    for_each((*territoryName).begin(), (*territoryName).end(), [](char & c) {
        c = ::tolower(c);
    });
    for(auto& territory: this->territories)
    {
        if (territory->getName() == *territoryName)
        {
            return territory;
        }
    }
    return nullptr;
}

vector<Territory*> Map::getTerritories()
{
    return this->territories;
}

vector<Continent*> Map::getContinents()
{
    return this->continents;
}

void Map::setValidFalse()
{
    this->isValid = false;
}

// Method that makes the following checks:
// 1) the map is a connected graph,
// 2) continents are connected sub-graphs, and
// 3) each country belongs to one and only one continent.
bool Map::validate()
{
    // The load function already checks if there is any adjacent territory that is not a territory
    // and if the territory is in a continent that does not exist.
    // If the map is not valid, skip the check.
    if (!this->isValid)
    {
        return false;
    }

    // Check if there is a duplicate Continent with the same name
    vector<std::string> continentNames = {};
    for (Continent* continent: this->continents)
    {
        const string continentName = continent->getName();
        if (std::find(continentNames.begin(), continentNames.end(), continentName) != continentNames.end())
        {
            cout << "Map has duplicate continent names: \"" << continentName << "\"" << endl;
            this->isValid = false;
            return false;
        }
        continentNames.push_back(continentName);
    }

    // Check if there is a duplicate Territory with the same name
    vector<std::string> territoryNames = {};
    for (Territory* territory: this->territories)
    {
        const string territoryName = territory->getName();
        if (territory->getContinent() == nullptr)
        {
            cout << "Map has a territory \"" << territoryName << "\" with no continent" << endl;
            this->isValid = false;
            return false;
        }
        if (std::find(territoryNames.begin(), territoryNames.end(), territoryName) != territoryNames.end())
        {
            cout << "Map has duplicate territory names: \"" << territoryName << "\"" << endl;
            this->isValid = false;
            return false;
        }
        territoryNames.push_back(territoryName);
        for (Territory* adjTerritory: territory->getAdjacentTerritories())
        {
            if (!adjTerritory)
            {
                cout << "Map has an adjacent territory for the \"" << territoryName << "\" territory that does not exists" << endl;
                this->isValid = false;
                return false;
            }
        }
        continentNames.push_back(territoryName);
    }

    return true;
}

ostream& operator<<(ostream& os, Map& map)
{
    return os << "Map is " << (map.isValid ? "valid" : "not valid");
}

Map &Map::operator=(const Map &map)
= default;


// ---------------------------------------------
// ------------- Continent Section -------------
// ---------------------------------------------

Continent::Continent(std::string name, int score)
{
    for_each(name.begin(), name.end(), [](char & c) {
        c = ::tolower(c);
    });
    this->name = std::move(name);
    this->score = score;
}

Continent::Continent(const Continent& continent)
{
    this->name = continent.name;
    this->score = continent.score;
}

Continent::~Continent() = default;

std::string Continent::getName()
{
    return name;
}

int Continent::getScore() const
{
    return score;
}

ostream& operator<<(ostream& os, Continent& continent)
{
    return os << "Continent " << continent.name << " with score of " << continent.score;
}

Continent &Continent::operator=(const Continent& continent)
= default;


// ---------------------------------------------
// ------------- Territory Section -------------
// ---------------------------------------------

Territory::Territory(string name, int coordinateX, int coordinateY, Continent* continent)
{
    for_each(name.begin(), name.end(), [](char & c) {
        c = ::tolower(c);
    });
    this->name = std::move(name);
    this->coordinateX = coordinateX;
    this->coordinateY = coordinateY;
    this->continent = continent;
    this->adjacentTerritories = {};
    this->ownedBy = nullptr;
    this->numberOfArmies = 0;
}

Territory::Territory(const Territory& territory, Continent* continent)
{
    this->coordinateX = territory.coordinateX;
    this->coordinateY = territory.coordinateY;
    this->name = territory.name;
    if (continent == nullptr)
    {
        this->continent = new Continent(*territory.continent);
    }
    else
    {
        this->continent = continent;
    }
    this->adjacentTerritories = {};
    for (Territory* adjTerritory: territory.adjacentTerritories)
    {
        this->adjacentTerritories.push_back(new Territory(*adjTerritory));
    }
    this->ownedBy = territory.ownedBy;
    this->numberOfArmies = territory.numberOfArmies;
}

Territory::~Territory() = default;

string Territory::getName()
{
    return this->name;
}

Continent *Territory::getContinent()
{
    return this->continent;
}

Player* Territory::getOwnedBy()
{
    return this->ownedBy;
}

int Territory::getNumberOfArmies()
{
    return this->numberOfArmies;
}

void Territory::addAdjacentTerritory(Territory *territory)
{
    this->adjacentTerritories.push_back(territory);
}

void Territory::addArmies(int numArmies)
{
    this->numberOfArmies = numberOfArmies + numArmies;
}

void Territory::removeArmies(int numArmies)
{
    this->numberOfArmies = numberOfArmies - numArmies;
		if (this->numberOfArmies < 0)
		{
			this->getOwnedBy()->removeTerritory(this);
			this->setOwnedBy(new Player(true), this->getNumberOfArmies() * 2);
		}
}

vector<Territory*> Territory::getAdjacentTerritories() {
    return this->adjacentTerritories;
}

// A territory is owned by a player and contain a number of armies.
void Territory::setOwnedBy(Player* ownedBy, int numberOfArmies)
{
	// Replace old owner
	if (this->ownedBy)
	{
		this->ownedBy->removeTerritory(this);
		ownedBy->addOwnedTerritory(this);
	}
	this->ownedBy = ownedBy;
	this->numberOfArmies = numberOfArmies;
}

void Territory::setNumberOfArmies(int numberOfArmies)
{
    this->numberOfArmies = numberOfArmies;
}

ostream& operator<<(ostream& os, Territory& territory)
{
    return os << "Territory " << territory.name
        << " with coordinates (" << territory.coordinateX << "," << territory.coordinateY << ")"
        << " belongs to " << territory.continent->getName()
        << " and has " << territory.numberOfArmies;
}

Territory &Territory::operator=(const Territory& territory)
= default;

//Overload < operator, comparison is based on number of armies
bool operator<(const Territory& t1, const Territory& t2)
{
    return t1.numberOfArmies < t2.numberOfArmies;
}

bool operator>(const Territory& t1, const Territory& t2)
{
    return t2.numberOfArmies < t1.numberOfArmies;
}


// ---------------------------------------------
// ------------- MapLoader Section -------------
// ---------------------------------------------

MapLoader::MapLoader() = default;

MapLoader::MapLoader(const MapLoader &mapLoader) = default;

MapLoader::~MapLoader() = default;

Map* MapLoader::load(const std::string& mapFileDir)
{
    cout << "Loading file: " << mapFileDir << endl;

    // Create a new Map
    Map* map = new Map();

    try {
        // Open a read stream to the file mapFileDir
        ifstream input(mapFileDir);
        string line;

        // Check if map file exists
        if (!input.good())
        {
            cout << "Map file not found: " << mapFileDir << endl
                << "Map will be set as empty" << endl;
        }

        // Loop through the lines of the map config
        while (getline(input, line))
        {
            // Continent section
            if (line == "[Continents]\r" || line == "[Continents]")
            {
                // Loop through the lines
                while (getline(input, line))
                {
                    if (line == "\r" || line.empty())
                    {
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
            else if (line == "[Territories]\r" || line == "[Territories]")
            {
                // Create a map that stores the Territory object reference with the names of the adjacent Territories
                std::map<Territory*, vector<string>> territoryAdjMap;
                // Loop through each line
                while (getline(input, line))
                {
                    // Skip empty lines
                    if (line == "\r" || line.empty())
                    {
                        continue;
                    }

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

                    // Loop through the remaining line with a delimiter ',' to get the names of the adjacent territories
                    string adjacentTerritoryName;
                    stringstream lineStream;
                    lineStream << line;
                    while (getline(lineStream, adjacentTerritoryName, ','))
                    {
                        if (adjacentTerritoryName.find('\r') != std::string::npos)
                        {
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
                            // Could not find territory
                            cout << "Map has an adjacent territory \"" << adjTerritoryName
                                << "\" for the \"" << pair.first->getName() << "\" territory that does not exists"
                                << endl;
                        }
                        pair.first->addAdjacentTerritory(adjTerritory);
                    }
                }

            }
        }
        input.close();
    } catch (const std::exception &exc) {
        std::cerr << exc.what();
        cout << "An error has occurred when creating the map from config file: " << mapFileDir << std::endl;
        map->setValidFalse();
    }
    const bool valid = map->validate();
    cout << std::boolalpha << mapFileDir << " is valid: " << valid << endl;
    return map;
}

ostream& operator<<(ostream& os, MapLoader& mapLoader)
{
    return os << "MapLoader";
}

MapLoader &MapLoader::operator=(const MapLoader& mapLoader)
= default;