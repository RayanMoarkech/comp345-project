//
// COMP345_PROJECT_PLAYERSTRATEGIES_H PlayerStrategies.h
//

#pragma once

#include <ostream>
    using std::ostream;
#include <string>
    using std::string;
#include <ostream>
		using std::vector;

class Player;
class Order;
class Territory;

// ---------------------------------------------
// ---------- PlayerStrategy Section -----------
// ---------------------------------------------

class PlayerStrategy {
protected:
		Player *_player;

public:
		PlayerStrategy();
		PlayerStrategy(Player *player);
		PlayerStrategy(const PlayerStrategy &playerStrategy);

		void setPlayer(Player *player);
		Player *getPlayer();

		virtual Order *issueOrder() = 0;
		virtual vector<Territory*> toAttack() = 0;
		virtual vector<Territory*> toDefend() = 0;

		friend ostream &operator<<(ostream &os, PlayerStrategy &playerStrategy);
		PlayerStrategy &operator=(const PlayerStrategy &playerStrategy);

		void playCard(string cardType);

		virtual ~PlayerStrategy();
};

// ---------------------------------------------
// ------- NeutralPlayerStrategy Section -------
// ---------------------------------------------

class NeutralPlayerStrategy : public PlayerStrategy {
public:
		NeutralPlayerStrategy();
		NeutralPlayerStrategy(Player *player);
		NeutralPlayerStrategy(const PlayerStrategy &playerStrategy);

		Order *issueOrder();
		vector<Territory*> toAttack();
		vector<Territory*> toDefend();

		//    friend ostream& operator<<(ostream& os, PlayerStrategies&
		//    playerStrategies); PlayerStrategies& operator=(const PlayerStrategies&
		//    playerStrategies);

		virtual ~NeutralPlayerStrategy();
};

// ---------------------------------------------
// ------BenevolentPlayerStrategy Section-------
// ---------------------------------------------

class BenevolentPlayerStrategy : public PlayerStrategy {
public:
		BenevolentPlayerStrategy();
		BenevolentPlayerStrategy(Player *player);
		BenevolentPlayerStrategy(const PlayerStrategy &playerStrategy);

		Order *issueOrder();
		vector<Territory*> toAttack();
		vector<Territory*> toDefend();

		//    friend ostream& operator<<(ostream& os, PlayerStrategies&
		//    playerStrategies); PlayerStrategies& operator=(const PlayerStrategies&
		//    playerStrategies);

		int toDefendIndex = 0;
		int toAdvanceIndex = 0;

		virtual ~BenevolentPlayerStrategy();
};

// ---------------------------------------------
// ----- AggressivePlayerStrategy Section ------
// ---------------------------------------------

class AggressivePlayerStrategy : public PlayerStrategy {
public:
		AggressivePlayerStrategy();
		AggressivePlayerStrategy(Player *player);
		AggressivePlayerStrategy(const PlayerStrategy &playerStrategy);

		Order *issueOrder();
		vector<Territory*> toAttack();
		vector<Territory*> toDefend();

		//    friend ostream& operator<<(ostream& os, PlayerStrategies&
		//    playerStrategies); PlayerStrategies& operator=(const PlayerStrategies&
		//    playerStrategies);

		virtual ~AggressivePlayerStrategy();
};

// ---------------------------------------------
// ----- HumanPlayerStrategy Section ------
// ---------------------------------------------

class HumanPlayerStrategy : public PlayerStrategy {
public:
		HumanPlayerStrategy();
		HumanPlayerStrategy(Player *player);
		HumanPlayerStrategy(const PlayerStrategy &playerStrategy);

		Order *issueOrder();
		vector<Territory*> toAttack();
		vector<Territory*> toDefend();

		//    friend ostream& operator<<(ostream& os, PlayerStrategies&
		//    playerStrategies); PlayerStrategies& operator=(const PlayerStrategies&
		//    playerStrategies);

		virtual ~HumanPlayerStrategy();
};

// ---------------------------------------------
// ------- CheaterPlayerStrategy Section -------
// ---------------------------------------------

class CheaterPlayerStrategy : public PlayerStrategy {
public:
		CheaterPlayerStrategy();
		CheaterPlayerStrategy(Player *player);
		CheaterPlayerStrategy(const PlayerStrategy &playerStrategy);

		Order *issueOrder();
		vector<Territory*> toAttack();
		vector<Territory*> toDefend();

		virtual ~CheaterPlayerStrategy();

private:
		bool attackedOncePerTurn = false;
};

