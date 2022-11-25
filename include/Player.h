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
    static int idCounter;
    int id;
    string name;
	vector<Territory*> ownedTerritories;
	std::list<Player*> negotiatingWith;
	Hand* playerHand;
	OrdersList* playerOrders;
    int armyUnits;
	vector<Territory*> toDefendList;
	vector<Territory*> toAttackList;
	bool conqueredTerritory;
	bool isNeutral;

public:
	Player();
	Player(string name, vector<Territory*> ownedTerritories, Hand* playerHand, OrdersList* playerOrders);
	Player(const Player& player);
	Player(bool isNeutral);
	~Player();
	Player& operator=(const Player& player);
	friend std::ostream& operator<<(std::ostream& os, const Player& p);
	void toDefend();
	void toAttack();
	Order* issueOrder();
	vector<Territory*> getOwnedTerritories();
	Hand* getPlayerHand();
	OrdersList* getPlayerOrders();
	void addTerritory(Territory* t);
	void removeTerritory(Territory* t);
    void ownTerritory(Territory *territory, int armyUnits);
	void addNegotiator(Player* p); //Adds a negotiating player
	void removeReinforcements(int numArmies);
	//May need to add a method to remove someone from the list of negotiators
	std::list<Player*> getNegotiatorList();
	void setConqueredTerritory(bool conqueredTerritory);
	bool getConqueredTerritory();
    string getName();
	void setName(string name);
    void setArmyUnits(int armyUnits);
    int getArmyUnits();
    void addOwnedTerritory(Territory* territory);
	vector<Territory*> getNeighbouringEnemyTerritories();
	vector<Territory*> getNeighbouringEnemyTerritories(Territory* t);
	vector<Territory*> getNeighbouringOwnedTerritories(Territory* t);
	vector<Territory*> getAttackList();
	void setAttackList(vector<Territory*> toAttackList);
	vector<Territory*> getDefendList();
	void setDefendList(vector<Territory*> toDefendList);
};

#endif //Player_H