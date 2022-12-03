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
		bool conqueredTerritory;
		bool isNeutral;
		PlayerStrategy *ps;
		GameEngine *gameEngine;

public:
		Player();
		Player(string name, vector<Territory *> ownedTerritories, Hand *playerHand, OrdersList *playerOrders);
		Player(const Player &player);
		Player(bool isNeutral);
		~Player();
		Player &operator=(const Player &player);
		friend std::ostream &operator<<(std::ostream &os, const Player &p);
		vector<Territory*> toDefend();
		vector<Territory*> toAttack();
		Order *issueOrder();
		vector<Territory *> getOwnedTerritories();
		Hand *getPlayerHand();
		OrdersList *getPlayerOrders();
		void addTerritory(Territory *t);
		void removeTerritory(Territory *t);
		void ownTerritory(Territory *territory, int armyUnits);
		void addNegotiator(Player *p); //Adds a negotiating player
		void removeReinforcements(int numArmies);
		std::list<Player *> getNegotiatorList();
		void setConqueredTerritory(bool conqueredTerritory);
		bool getConqueredTerritory();
		string getName();
		void setArmyUnits(int armyUnits);
		int getArmyUnits();
		void addOwnedTerritory(Territory *territory);
		vector<Territory *> getNeighbouringEnemyTerritories();
		vector<Territory *> getNeighbouringEnemyTerritories(Territory *t);
		vector<Territory *> getNeighbouringOwnedTerritories(Territory *t);
		bool ownsCard(string cardType);
		void setPlayerStrategy(PlayerStrategy *ps);
		GameEngine *getGameEngine();
		void setGameEngine(GameEngine *gameEngine);
};

#endif //Player_H