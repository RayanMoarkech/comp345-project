//
// COMP345_PROJECT_PLAYER_H Player.h
//

#ifndef Player_H
#define Player_H

#include <vector>
using std::vector;

#include "../include/Map.h"

//import the following when merging features
class Order;
class OrdersList;
class Deck;

class Player
{
private:
	vector<Territory*> ownedTerritories;
	Deck* playerDeck;
	OrdersList* playerOrders;

public:
	Player();
	Player(vector<Territory*> ownedTerritories, Deck* playerDeck, OrdersList* playerOrders);
	Player(const Player& player);
	vector<Territory*> toDefend();
	vector<Territory*> toAttack();
	void issueOrder();
};

#endif //Player_H