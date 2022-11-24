//
// COMP345_PROJECT_PLAYERSTRATEGIES_CPP PlayerStrategies.CPP
//

#include "../include/PlayerStrategies.h"
#include "../include/Player.h"

#include <iostream>
    using std::cout;
	using std::cin;
    using std::endl;
#include <vector>
    using std::vector;

class Territory;

//Static Methods
static bool isIn(vector<Territory*> territoryVector, Territory* territory)
{
	for (Territory* t : territoryVector)
	{
		if (t == territory)
		{
			return true;
		}
	}
	return false;
}


// ---------------------------------------------
// ---------- PlayerStrategy Section -----------
// ---------------------------------------------

// Constructors

PlayerStrategy::PlayerStrategy()
{
    this->_player = nullptr;
}

PlayerStrategy::PlayerStrategy(Player *player)
{
    this->_player = player;
}

PlayerStrategy::PlayerStrategy(const PlayerStrategy &playerStrategy)
{
    this->_player = playerStrategy._player;
}

// Getters and Setters

void PlayerStrategy::setPlayer(Player *player)
{
    this->_player = player;
}

Player *PlayerStrategy::getPlayer()
{
    return this->_player;
}

// Overloading

ostream& operator<<(ostream& os, PlayerStrategy& playerStrategy)
{
    cout << "PlayerStrategy with ";
    if (playerStrategy._player)
        cout << "player " << playerStrategy._player << endl;
    else
        cout << "no player" << endl;
    return cout;
}

PlayerStrategy &PlayerStrategy::operator=(const PlayerStrategy &playerStrategy) = default;

// Destructor

PlayerStrategy::~PlayerStrategy() = default;


// ---------------------------------------------
// ------- NeutralPlayerStrategy Section -------
// ---------------------------------------------

// Constructors

NeutralPlayerStrategy::NeutralPlayerStrategy(): PlayerStrategy() {}

NeutralPlayerStrategy::NeutralPlayerStrategy(Player *player): PlayerStrategy(player) {}

NeutralPlayerStrategy::NeutralPlayerStrategy(const PlayerStrategy &playerStrategy): PlayerStrategy(playerStrategy) {}

// Functionalities

// never issues any order
PlayerStrategy* NeutralPlayerStrategy::issueOrder()
{
    return this;
}

PlayerStrategy* NeutralPlayerStrategy::toAttack()
{
    // TODO: not sure what to put here
    return this;
}

// is attacked, it becomes an Aggressive player
PlayerStrategy* NeutralPlayerStrategy::toDefend()
{
    return new AggressivePlayerStrategy(*this);
}

// Destructor

NeutralPlayerStrategy::~NeutralPlayerStrategy() = default;


// ---------------------------------------------
// ----- AggressivePlayerStrategy Section ------
// ---------------------------------------------

// Constructors

AggressivePlayerStrategy::AggressivePlayerStrategy(): PlayerStrategy() {}

AggressivePlayerStrategy::AggressivePlayerStrategy(Player *player): PlayerStrategy(player) {}

AggressivePlayerStrategy::AggressivePlayerStrategy(const PlayerStrategy &playerStrategy): PlayerStrategy(playerStrategy) {}

// Functionalities

PlayerStrategy* AggressivePlayerStrategy::issueOrder() {
    //TODO: complete part
    return this;
}

PlayerStrategy* AggressivePlayerStrategy::toAttack()
{
    //TODO: computer player that focuses on attack
    // deploys or advances armies on its strongest country,
    // then always advances to enemy territories until it cannot do so anymore
    return this;
}

PlayerStrategy* AggressivePlayerStrategy::toDefend()
{
    //TODO: complete part
    return this;
}

// Destructor

AggressivePlayerStrategy::~AggressivePlayerStrategy() = default;

// ---------------------------------------------
// ----- HumanPlayerStrategy Section ------
// ---------------------------------------------

// Constructors

HumanPlayerStrategy::HumanPlayerStrategy() : PlayerStrategy() {}

HumanPlayerStrategy::HumanPlayerStrategy(Player* player) : PlayerStrategy(player) {}

HumanPlayerStrategy::HumanPlayerStrategy(const PlayerStrategy& playerStrategy) : PlayerStrategy(playerStrategy) {}

// Functionalities

PlayerStrategy* HumanPlayerStrategy::issueOrder() {
    //TODO: complete part
    return this;
}

PlayerStrategy* HumanPlayerStrategy::toAttack()
{
    //TODO: computer player that focuses on attack
    // deploys or advances armies on its strongest country,
    // then always advances to enemy territories until it cannot do so anymore
    return this;
}

PlayerStrategy* HumanPlayerStrategy::toDefend()
{
    //TODO: complete part
	cout << "HumanPlayerStrategy toDefend" << endl;
	vector<Territory*> territoriesToDefend;
	cout << "Which territories should be defended in priority?" << endl;
	int counter = 1;
	for (Territory* t : this->getPlayer()->getOwnedTerritories())
	{
		cout << counter << " - " << t->getName() << endl;
		counter++;
	}
	cout << "Enter number corresponding to territory to add to list. Enter 0 to stop." << endl;
	int territoryToDefend = -1;
	while (territoryToDefend != 0)
	{
		cin >> territoryToDefend;
		if (territoryToDefend == 0)
		{
			break;
		}
		else if (territoryToDefend < 0 || territoryToDefend > this->getPlayer()->getOwnedTerritories().size())
		{
			cout << "Not a valid number." << endl;
			cout << "Enter number corresponding to territory to add to list. Enter 0 to stop." << endl;
		}
		else
		{
			if (!isIn(territoriesToDefend, this->getPlayer()->getOwnedTerritories().at(territoryToDefend - 1)))
			{
				territoriesToDefend.push_back(this->getPlayer()->getOwnedTerritories().at(territoryToDefend - 1));
			}
			else
			{
				cout << "This territory is already in the to defend list." << endl;
			}
		}
	}
	cout << "prioritized list of territories to defend: " << endl;
	counter = 1;
	for (Territory* t : territoriesToDefend)
	{
		cout << counter << " - " << t->getName() << endl;
		counter++;
	}
    return this;
}

// Destructor

HumanPlayerStrategy::~HumanPlayerStrategy() = default;
