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
	using std::list;
#include <string>
	using std::string;

class Territory;
class OrdersList;
class Order;
class PlayerStrategy;
class GameEngine;
class Hand;
class Card;

class Player {
private:
		static int idCounter;
		int id;
		string name;
		vector<Territory *> ownedTerritories;
		list<Player *> negotiatingWith;
		Hand *playerHand;
		OrdersList *playerOrders;
		int armyUnits;
		vector<Territory *> toDefendList;
		vector<Territory *> toAttackList;
		bool conqueredTerritory;
		bool isNeutral;
		PlayerStrategy *ps;

public:
		Player();
		Player(string name, vector<Territory *> ownedTerritories, Hand *playerHand, OrdersList *playerOrders, PlayerStrategy* ps = nullptr);
		Player(const Player &player);
		Player(bool isNeutral);
		~Player();
		Player &operator=(const Player &player);
		friend std::ostream &operator<<(std::ostream &os, const Player &p);
		void toDefend();
		void toAttack();
		Order *issueOrder();
		vector<Territory *> getOwnedTerritories();
		Hand *getPlayerHand();
		OrdersList *getPlayerOrders();
		void addTerritory(Territory *t);
		void removeTerritory(Territory *t);
		void ownTerritory(Territory *territory, int armyUnits);
		void addNegotiator(Player *p); //Adds a negotiating player
		void removeReinforcements(int numArmies);
		//May need to add a method to remove someone from the list of negotiators
		std::list<Player *> getNegotiatorList();
		void setConqueredTerritory(bool conqueredTerritory);
		bool getConqueredTerritory();
		string getName();
		void setName(string name);
		void setArmyUnits(int armyUnits);
		int getArmyUnits();
		void addOwnedTerritory(Territory *territory);
		vector<Territory *> getNeighbouringEnemyTerritories();
		vector<Territory *> getNeighbouringEnemyTerritories(Territory *t);
		vector<Territory *> getNeighbouringOwnedTerritories(Territory *t);
		vector<Territory *> getAttackList();
		void setAttackList(vector<Territory *> toAttackList);
		vector<Territory *> getDefendList();
		void setDefendList(vector<Territory *> toDefendList);
		bool ownsCard(string cardType);
		void removeCardFromHand(string cardType);
		PlayerStrategy *getPlayerStrategy();
		void setPlayerStrategy(PlayerStrategy *ps);
};

#endif //Player_H