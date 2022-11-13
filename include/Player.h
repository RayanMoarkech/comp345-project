//
// COMP345_PROJECT_PLAYER_H Player.h
//

#ifndef Player_H
#define Player_H

#include <vector>
    using std::vector;
#include <ostream>
	using std::ostream;

#include "../include/Map.h"
#include "../include/Orders.h"

//import the following when merging features
class Hand;

class Player
{
private:
    static int idCounter;
    int id;
    string name;
	vector<Territory*> ownedTerritories;
	Hand* playerHand;
	OrdersList* playerOrders;
    int armyUnits;
	vector<Territory*> toDefendList;
	vector<Territory*> toAttackList;

public:
	Player();
	Player(string name, vector<Territory*> ownedTerritories, Hand* playerHand, OrdersList* playerOrders);
	Player(const Player& player);
	~Player();
	Player& operator=(const Player& player);
	friend std::ostream& operator<<(std::ostream& os, const Player& p);
	vector<Territory*> toDefend();
	vector<Territory*> toAttack();
	Order* issueOrder();
	vector<Territory*> getOwnedTerritories();
	Hand* getPlayerHand();
	OrdersList* getPlayerOrders();
    string getName();
	void setName(string name);
    void setArmyUnits(int armyUnits);
    int getArmyUnits();
    void addOwnedTerritory(Territory* territory);
	vector<Territory*> getNeighbouringTerritories();
	vector<Territory*> getAttackList();
	vector<Territory*> getDefendList();
};

#endif //Player_H