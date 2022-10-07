#include "../../include/Map.h"
#include "../../include/Player.h"
#include <vector>
using std::vector;

#include <iostream>
using std::cout;
using std::endl;

void testPlayers()
{
	srand(time(NULL)); //change seed to get random numbers every run
	Player* player = new Player();
	vector<Territory*> territoriesToDefend = player->toDefend();
	vector<Territory*> territoriesToAttack = player->toAttack();
}
