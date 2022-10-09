#include "../../include/Player.h"
#include "../../include/Orders.h"
#include <vector>
using std::vector;

#include <iostream>
using std::cout;
using std::endl;

void testPlayers()
{

	cout << "\nTesting Player Functions\nPlayer 1" << endl;
	srand(time(NULL)); //change seed to get random numbers every run
	Player* player = new Player();
	vector<Territory*> territoriesToDefend = player->toDefend();

	cout << "\nPlayer 2" << endl;
	Player* player2 = new Player();
	vector<Territory*> territoriesToAttack = player->toAttack();
	player2->issueOrder();
	player2->issueOrder();

	cout << "\nPlayer 3 (Copy of Player 2)" << endl;
	Player* player3 = new Player(*player2);
	cout << "\nPlayer 3 issues new order: " << endl;
	player3->issueOrder();
	//Player 2 issues new order and outputs size to show that Player 3 is in fact a deep copy
	cout << "\nPlayer 2 issues new order: " << endl;
	player2->issueOrder();
}
