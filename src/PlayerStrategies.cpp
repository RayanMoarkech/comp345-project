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
	cout << "----------------------------------" << endl;
	cout << this->getPlayer()->getName() << "'s Turn" << endl;
	cout << "----------------------------------" << endl;

	//To Attack
	if (this->getPlayer()->getAttackList().size() == 0)
	{
		this->toAttack();
		return NULL;
	}
	//To Defend
	if (this->getPlayer()->getDefendList().size() == 0)
	{
		this->toDefend();
		return NULL;
	}

	//Deploy on toDefend territories, will not be able to issue other orders until
	//all armies are deployed
	if (this->getPlayer()->getArmyUnits() != 0)
	{
		cout << "Choose territory and how many many army units to deploy." << endl;
		int counter = 1;
		for (Territory* t : this->getPlayer()->getDefendList())
		{
			cout << counter << " - " << t->getName() << endl;
			counter++;
		}
		int territoryToDefend = -1;
		int armyUnitsToDeploy;

		cout << "Total remaining of army units to deploy: " << this->getPlayer()->getArmyUnits() << endl;
		cout << "Choose territory: ";
		cin >> territoryToDefend;

		if (territoryToDefend <= 0 || territoryToDefend > this->getPlayer()->getDefendList().size())
		{
			cout << "Not a valid number." << endl;
			cout << "Enter number corresponding to territory to add to list. Enter 0 to stop." << endl;
		}
		else
		{
			cout << "Number of army units to deploy to " << this->getPlayer()->getDefendList().at(territoryToDefend - 1)->getName() << ": ";
			cin >> armyUnitsToDeploy;
			while (armyUnitsToDeploy > this->armyUnits || armyUnitsToDeploy <= 0)
			{
				cout << "Not a valid number of army units" << endl;
				cout << "Number of army units to deploy to " << this->getPlayer()->getDefendList().at(territoryToDefend - 1)->getName() << ": ";
				cin >> armyUnitsToDeploy;
			}
			this->armyUnits -= armyUnitsToDeploy;
			//return new Deploy(this, this->toDefendList.at(territoryToDefend - 1), armyUnitsToDeploy);
		}

	//Advance (Defend)
	/*if (this->toDefendList.size() != 5)
	{
		Territory* targetTerritory = this->toDefendList.at(0);
		this->toDefendList.erase(this->toDefendList.begin());
		int index = (rand() % this->toDefendList.size());
		Territory* sourceTerritory = this->toDefendList.at(index);
		int armiesToAdvance = sourceTerritory->getNumberOfArmies();


		cout << endl;
		cout << "Defense: " << this->getName() << " advances " << armiesToAdvance << " armies from " <<
			sourceTerritory->getName() << " to " << targetTerritory->getName() << endl;
		cout << endl;
		return new Advance(this, sourceTerritory, targetTerritory, armiesToAdvance);
	}
	if (this->toAttackList.size() != 5)
	{
		Territory* targetTerritory = this->toAttackList.at(0);
		this->toAttackList.erase(this->toAttackList.begin());
		int index = (rand() % this->ownedTerritories.size());
		Territory* sourceTerritory = this->ownedTerritories.at(index);
		int armiesToAdvance = sourceTerritory->getNumberOfArmies();

		cout << endl;
		cout << "Attack: " << this->getName() << " advances " << armiesToAdvance << " armies from " <<
			sourceTerritory->getName() << " to " << targetTerritory->getName() << endl;
		cout << endl;
		return new Advance(this, sourceTerritory, targetTerritory, armiesToAdvance);
	}
	if (this->getPlayerHand()->cards.size() != 0)
	{
		string cardType = this->getPlayerHand()->cards.at(0)->getCardType();
		cout << endl;
		cout << "Player played card: " << cardType << endl;
		if (cardType == "Bomb")
		{
			int index = (rand() % this->toAttackList.size());
			Territory* toBomb = this->toAttackList.at(index);
			cout << this->getName() << " will bomb " << toBomb->getName() << endl;
			cout << endl;
			return new Bomb(this, toBomb);
		}
		if (cardType == "Blockade")
		{
			int index = (rand() % this->toDefendList.size());
			Territory* toBlockade = this->toDefendList.at(index);
			cout << this->getName() << " will blockade " << toBlockade->getName() << endl;
			cout << endl;
			return new Blockade(this, toBlockade);
		}
		if (cardType == "Blockade")
		{
			int index = (rand() % this->toDefendList.size());
			Territory* toBlockade = this->toDefendList.at(index);
			cout << this->getName() << " will blockade " << this->toDefendList.at(index)->getName() << endl;
			cout << endl;
			return new Blockade(this, toBlockade);
		}
		if (cardType == "Airlift")
		{
			int index1 = (rand() % this->toDefendList.size());
			Territory* target = this->toDefendList.at(index1);
			int index2 = (rand() % this->toDefendList.size());
			Territory* source = this->toDefendList.at(index2);
			cout << this->getName() << " will airlift 5 armies from " << source->getName() <<
				" to " << target->getName() << endl;
			cout << endl;
			return new Airlift(this, source, target, 5);
		}
		return NULL;
	}
	cout << endl;
	cout << this->getName() << " has no more orders." << endl;
	cout << endl;
	return NULL;*/
    return this;
}

PlayerStrategy* HumanPlayerStrategy::toAttack()
{
	vector<Territory*> territoriesToAttack;
	cout << "Which neighbouring territories should be attacked in priority?" << endl;
	vector<Territory*> neighbouringTerritories = this->getPlayer()->getNeighbouringTerritories();
	int counter = 1;
	for (Territory* t : neighbouringTerritories)
	{
		cout << counter << " - " << t->getName() << endl;
		counter++;
	}
	cout << "Enter number corresponding to territory to add to list. Enter 0 to stop." << endl;
	int territoryToAttack = -1;
	while (territoryToAttack != 0)
	{
		cin >> territoryToAttack;
		if (territoryToAttack == 0)
		{
			break;
		}
		else if (territoryToAttack < 0 || territoryToAttack > neighbouringTerritories.size())
		{
			cout << "Not a valid number." << endl;
			cout << "Enter number corresponding to territory to add to list. Enter 0 to stop." << endl;
		}
		else
		{
			if (!isIn(territoriesToAttack, neighbouringTerritories.at(territoryToAttack - 1)))
			{
				territoriesToAttack.push_back(neighbouringTerritories.at(territoryToAttack - 1));
			}
			else
			{
				cout << "This territory is already in the to attack list." << endl;
			}
		}
	}
	cout << "prioritized list of territories to attack: " << endl;
	counter = 1;
	for (Territory* t : territoriesToAttack)
	{
		cout << counter << " - " << t->getName() << endl;
		counter++;
	}
    return this;
}

PlayerStrategy* HumanPlayerStrategy::toDefend()
{
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
