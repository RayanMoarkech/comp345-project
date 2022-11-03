//
// COMP345_PROJECT_PLAYER_H Player.h
//

#ifndef Player_H
#define Player_H

#include <vector>
    using std::vector;
#include <ostream>
	using std::ostream;
#include <list>

#include "../include/Map.h"
#include "../include/Orders.h"

//import the following when merging features
class Hand;

class Player
{
private:
	vector<Territory*> ownedTerritories;
	std::list<Player*> negotiatingWith;
	Hand* playerHand;
	OrdersList* playerOrders;

public:
	Player();
	Player(vector<Territory*> ownedTerritories, Hand* playerHand, OrdersList* playerOrders);
	Player(const Player& player);
	~Player();
	Player& operator=(const Player& player);
	friend std::ostream& operator<<(std::ostream& os, const Player& p);
	vector<Territory*> toDefend();
	vector<Territory*> toAttack();
	void issueOrder();
	vector<Territory*> getOwnedTerritories();
	Hand* getPlayerHand();
	OrdersList* getPlayerOrders();
	void addTerritory(Territory* t);
	void removeTerritory(Territory* t); //TODO
	void addNegotiator(Player* p); //Adds a negotiating player
	//May need to add a method to remove someone from the list of negotiators
	std::list<Player*> getNegotiatorList();
};

#endif //Player_H