//
// COMP345_PROJECT_PLAYERDRIVER_CPP PlayerDriver.cpp
//

#include "../../include/Map.h"
#include "../../include/Player.h"

#include <vector>
    using std::vector;

#include <iostream>
    using std::cout;
    using std::endl;

void testPlayers()
{
    cout << endl
        << "------------------------------------------------------"
        << endl
        << "Test Players" << endl
        << "------------------------------------------------------" << endl
        << endl;

	srand(time(NULL)); //change seed to get random numbers every run
	Player* player = new Player();
	vector<Territory*> territoriesToDefend = player->toDefend();
	vector<Territory*> territoriesToAttack = player->toAttack();
}
