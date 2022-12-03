//
// COMP345_PROJECT_PLAYERSTRATEGIES_H PlayerStrategies.h
//

#pragma once

#include <ostream>
    using std::ostream;
#include <string>
    using std::string;

class Player;
class Order;

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
		virtual void toAttack() = 0;
		virtual void toDefend() = 0;

		friend ostream &operator<<(ostream &os, PlayerStrategy &playerStrategy);
		PlayerStrategy &operator=(const PlayerStrategy &playerStrategy);

		void playCard(string cardType);

		~PlayerStrategy();
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
		void toAttack();
		void toDefend();

		//    friend ostream& operator<<(ostream& os, PlayerStrategies&
		//    playerStrategies); PlayerStrategies& operator=(const PlayerStrategies&
		//    playerStrategies);

		~NeutralPlayerStrategy();
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
		void toAttack();
		void toDefend();

		//    friend ostream& operator<<(ostream& os, PlayerStrategies&
		//    playerStrategies); PlayerStrategies& operator=(const PlayerStrategies&
		//    playerStrategies);

		int toDefendIndex = 0;
		int toAdvanceIndex = 0;

		~BenevolentPlayerStrategy();
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
		void toAttack();
		void toDefend();

		//    friend ostream& operator<<(ostream& os, PlayerStrategies&
		//    playerStrategies); PlayerStrategies& operator=(const PlayerStrategies&
		//    playerStrategies);

		~AggressivePlayerStrategy();
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
		void toAttack();
		void toDefend();

		//    friend ostream& operator<<(ostream& os, PlayerStrategies&
		//    playerStrategies); PlayerStrategies& operator=(const PlayerStrategies&
		//    playerStrategies);

		~HumanPlayerStrategy();
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
		void toAttack();
		void toDefend();

		~CheaterPlayerStrategy();

		bool isAttackedOncePerTurn() const;
		void setAttackedOncePerTurn(bool attackedOncePerTurn);

private:
		bool attackedOncePerTurn = false;
};

