//
// COMP345_PROJECT_PLAYERSTRATEGIES_CPP PlayerStrategies.CPP
//

#include "../include/PlayerStrategies.h"
#include "../include/Player.h"
#include "../include/Map.h"

#include <iostream>
    using std::cout;
	using std::cin;
    using std::endl;
#include <vector>
    using std::vector;
#include <algorithm>
	using std::sort;

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
// -------BenevolentPlayerStrategy Sectio-------
// ---------------------------------------------

// Constructors

BenevolentPlayerStrategy::BenevolentPlayerStrategy() : PlayerStrategy() {}

BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player* player) : PlayerStrategy(player) {}

BenevolentPlayerStrategy::BenevolentPlayerStrategy(const PlayerStrategy& playerStrategy) : PlayerStrategy(playerStrategy) {}

// Functionalities

PlayerStrategy* BenevolentPlayerStrategy::issueOrder()
{
	cout << endl;
	cout << "----------------------------------" << endl;
	cout << this->getPlayer()->getName() << "'s Turn - Type: Benevolant" << endl;
	cout << "----------------------------------" << endl;
	cout << endl;

	//To Defend
	if (this->getPlayer()->getDefendList().size() == 0)
	{
		this->toDefend();
		return this;
	}

	//Deploy on toDefend territories, will not be able to issue other orders until
	//all armies are deployed
	//Deploys to weakest countries in priority
	if (this->getPlayer()->getArmyUnits() != 0)
	{
		cout << "Total remaining of army units to deploy: " << this->getPlayer()->getArmyUnits() << endl;
		// Divide army units by 3 to not deploy all armies to a single territory
		// Every turn this number will get smaller, proportionally deploying more
		// armies to the weakest countries.
		int armiesToDeploy = this->getPlayer()->getArmyUnits() / 3;
		if (armiesToDeploy == 0) { armiesToDeploy = 1; };
		Territory* weakestCountry = this->getPlayer()->getDefendList().at(this->toDefendIndex);
		cout << this->getPlayer()->getName() << " (Benevolant) will deploy " << armiesToDeploy <<
			" armies to territory " << this->getPlayer()->getDefendList().at(this->toDefendIndex)->getName() << endl;
		this->toDefendIndex++;
		this->getPlayer()->setArmyUnits(this->getPlayer()->getArmyUnits() - armiesToDeploy);
		//return new Deploy(this->getPlayer(), weakestCountry, armiesToDeploy);
		return this;
	}

	if (this->getPlayer()->getArmyUnits() == 0)
	{
		//Only plays Blockade and Airlift cards (Defensive cards)
		if (this->getPlayer()->ownsCard("Blockade"))
		{
			Territory* weakestCountry = this->getPlayer()->getDefendList().at(0);
			cout << this->getPlayer()->getName() << " will play a Blockade card." << endl;
			cout << this->getPlayer()->getName() << " will blockade territory " << weakestCountry->getName() << endl;
			//return new Blockade(this, weakestCountry);
		}
		if (this->getPlayer()->ownsCard("Airlift"))
		{
			Territory* weakestCountry = this->getPlayer()->getDefendList().front();
			Territory* strongestCountry = this->getPlayer()->getDefendList().back();
			int armiesToAirlift = strongestCountry->getNumberOfArmies() / 2;
			cout << this->getPlayer()->getName() << " will play a Airlift card." << endl;
			cout << this->getPlayer()->getName() << " will airlift " << armiesToAirlift << " armies to " 
				<< weakestCountry->getName() << " from " << strongestCountry->getName() << endl;
			//return new Airlift(this, strongestCountry, weakestCountry, armiesToAirlift);
		}

		// Only goes down a third of the list of countries to defend
		if (this->toAdvanceIndex != (this->getPlayer()->getDefendList().size() / 3))
		{
			// Advance (Defensive)
			Territory* weakestCountry = this->getPlayer()->getDefendList().at(this->toAdvanceIndex);
			vector<Territory*> adjacentOwnedCountries = this->getPlayer()->getNeighbouringOwnedTerritories(weakestCountry);
		
			//sort from strongest to weakest
			sort(adjacentOwnedCountries.begin(), adjacentOwnedCountries.end(),
				[](const Territory* t1, const Territory* t2) {return *t1 > *t2; });
			Territory* strongestCountry = adjacentOwnedCountries.front();
			int armiesToAdvance = strongestCountry->getNumberOfArmies() / 3;
			this->toAdvanceIndex++;
			cout << this->getPlayer()->getName() << " will advance " << armiesToAdvance << " armies to "
				<< weakestCountry->getName() << " from " << strongestCountry->getName() << endl;
			//return new Advance(this->getPlayer(), strongestCountry, weakestCountry, armiesToAdvance);
			return this;
		}
	}
	cout << this->getPlayer()->getName() << " has no more orders to issue." << endl;
	return this;
}

PlayerStrategy* BenevolentPlayerStrategy::toAttack()
{
	
	return this;
}


PlayerStrategy* BenevolentPlayerStrategy::toDefend()
{
	// Advances armies on weakest countries
	// Sort Player toDefend list by weakest to strongest country
	vector<Territory*> toDefend = this->_player->getOwnedTerritories();

	sort(toDefend.begin(), toDefend.end(), 
		[](const Territory* t1, const Territory* t2) {return *t1 < *t2; });
	cout << "Prioritized list of territories to defend: " << endl;
	cout << endl;
	for (Territory* t : toDefend)
	{
		cout << t->getName() << ": " << t->getNumberOfArmies() << " armies" << endl;
	}
	this->_player->setDefendList(toDefend);
	return this;
}

// Destructor

BenevolentPlayerStrategy::~BenevolentPlayerStrategy() = default;


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
	cout << endl;
	cout << "----------------------------------" << endl;
	cout << this->getPlayer()->getName() << "'s Turn - Type: Human" << endl;
	cout << "----------------------------------" << endl;
	cout << endl;

	//To Defend
	if (this->getPlayer()->getDefendList().size() == 0)
	{
		this->toDefend();
		return this;
	}

	//To Attack
	if (this->getPlayer()->getAttackList().size() == 0)
	{
		this->toAttack();
		return this;
	}

	//Deploy on toDefend territories, will not be able to issue other orders until
	//all armies are deployed
	if (this->getPlayer()->getArmyUnits() != 0)
	{
		cout << "Choose territory and how many many army units to deploy." << endl;
		cout << endl;
		int counter = 1;
		for (Territory* t : this->getPlayer()->getDefendList())
		{
			cout << counter << " - " << t->getName() << endl;
			counter++;
		}
		cout << endl;
		int territoryToDefend = -1;
		int armyUnitsToDeploy;

		cout << "Total remaining of army units to deploy: " << this->getPlayer()->getArmyUnits() << endl;
		cout << "Choose territory: ";
		cin >> territoryToDefend;

		if (territoryToDefend <= 0 || territoryToDefend > this->getPlayer()->getDefendList().size())
		{
			cout << "Not a valid number." << endl;
			cout << endl;
			cout << "Enter number corresponding to territory to add to list. Enter 0 to stop." << endl;
		}
		else
		{
			cout << "Number of army units to deploy to " << this->getPlayer()->getDefendList().at(territoryToDefend - 1)->getName() << ": ";
			cin >> armyUnitsToDeploy;
			while (armyUnitsToDeploy > this->getPlayer()->getArmyUnits() || armyUnitsToDeploy <= 0)
			{
				cout << "Not a valid number of army units" << endl;
				cout << endl;
				cout << "Number of army units to deploy to " << this->getPlayer()->getDefendList().at(territoryToDefend - 1)->getName() << ": ";
				cin >> armyUnitsToDeploy;
			}
			this->getPlayer()->setArmyUnits(this->getPlayer()->getArmyUnits() - armyUnitsToDeploy);
			//return new Deploy(this, this->toDefendList.at(territoryToDefend - 1), armyUnitsToDeploy);
			return this;
		}
	}

	if (this->getPlayer()->getArmyUnits() == 0)
		{
			cout << "1 - Defend: Advance armies to your own territory" << endl;
			cout << "2 - Attack: Advance armies to an enemy territory" << endl;
			cout << "3 - Bomb Card" << endl;
			cout << "4 - Blockade Card" << endl;
			cout << "5 - Airlift Card" << endl;
			cout << "6 - Negociate Card" << endl;
			cout << "0 - End Issuing Orders" << endl;
			int orderType = -1;
			cin >> orderType;
			while (orderType < 0 && orderType > 6)
			{
				cout << "Not a valid option" << endl;
				cout << endl;
				cout << "1 - Defend: Advance armies to your own territory" << endl;
				cout << "2 - Attack: Advance armies to an enemy territory" << endl;
				cout << "3 - Bomb Card" << endl;
				cout << "4 - Blockade Card" << endl;
				cout << "5 - Airlift Card" << endl;
				cout << "6 - Negociate Card" << endl;
				cout << "0 - Not issuing one more order" << endl;
				cin >> orderType;
			}

			if (orderType == 1)
			{
				cout << endl;
				cout << "What territory do you want to defend?" << endl;
				int counter = 1;
				for (Territory* t : this->_player->getDefendList())
				{
					cout << counter << " - " << t->getName() << endl;
					counter++;
				}
				int territoryToDefend;
				cin >> territoryToDefend;
				while (territoryToDefend < 0 || territoryToDefend > this->_player->getDefendList().size())
				{
					cout << "Not a valid number." << endl;
					cout << endl;
					cout << "What territory do you want to defend?" << endl;
					cin >> territoryToDefend;
				}

				vector<Territory*> sourceTerritories = this->_player->getNeighbouringOwnedTerritories(this->_player->getDefendList().at(territoryToDefend - 1));

				if (sourceTerritories.size() == 0)
				{
					cout << "This territory has no adjacent territories owned by you." << endl;
					//return NULL;
				}
				else
				{
					cout << endl;
					cout << "What territory is the source of armies?" << endl;
					counter = 1;
					for (Territory* t : sourceTerritories)
					{
						cout << counter << " - " << t->getName() << endl;
						counter++;
					}
					int sourceTerritory;
					cin >> sourceTerritory;
					while (sourceTerritory < 0 || sourceTerritory >sourceTerritories.size())
					{
						cout << "Not a valid number." << endl;
						cout << endl;
						cout << "What territory is the source of armies?" << endl;
						cin >> sourceTerritory;
					}
					int armiesToAdvance;
					cout << endl;
					cout << "How many armies to advance?" << endl;
					cin >> armiesToAdvance;
					//return new Advance(this, this->_player->getOwnedTerritories().at(sourceTerritory - 1),
					//	this->_player->getDefendList().at(territoryToDefend - 1), armiesToAdvance);
				}
				return this;
			}

			if (orderType == 2)
			{
				cout << endl;
				cout << "What territory do you want to attack?" << endl;
				int counter = 1;
				for (Territory* t : this->_player->getAttackList())
				{
					cout << counter << " - " << t->getName() << endl;
					counter++;
				}
				int territoryToAttack;
				cin >> territoryToAttack;
				while (territoryToAttack < 0 || territoryToAttack > this->_player->getAttackList().size())
				{
					cout << "Not a valid number." << endl;
					cout << endl;
					cout << "What territory do you want to attack?" << endl;
					cin >> territoryToAttack;
				}

				vector<Territory*> sourceTerritories = this->_player->getNeighbouringOwnedTerritories(this->_player->getAttackList().at(territoryToAttack - 1));

				cout << endl;
				cout << "What territory is the source of armies?" << endl;
				counter = 1;
				for (Territory* t : sourceTerritories)
				{
					cout << counter << " - " << t->getName() << endl;
					counter++;
				}
				int sourceTerritory;
				cin >> sourceTerritory;
				while (sourceTerritory < 0 || sourceTerritory > sourceTerritories.size())
				{
					cout << "Not a valid number." << endl;
					cout << endl;
					cout << "What territory is the source of armies?" << endl;
					cin >> sourceTerritory;
				}
				int armiesToAdvance;
				cout << endl;
				cout << "How many armies to advance?" << endl;
				cin >> armiesToAdvance;
				//return new Advance(this, this->_player->getOwnedTerritories().at(sourceTerritory - 1),
				//	this->_player->getAttackList().at(territoryToAttack - 1), armiesToAdvance);
				return this;
			}

			if (orderType == 3)
			{
				if (this->_player->ownsCard("Bomb"))
				{
					cout << endl;
					cout << "What territory do you want to bomb?" << endl;
					int counter = 1;
					for (Territory* t : this->_player->getAttackList())
					{
						cout << counter << " - " << t->getName() << endl;
						counter++;
					}
					int territoryToBomb;
					cin >> territoryToBomb;
					while (territoryToBomb < 0 || territoryToBomb > this->_player->getAttackList().size())
					{
						cout << "Not a valid number." << endl;
						cout << endl;
						cout << "What territory do you want to bomb?" << endl;
						cin >> territoryToBomb;
					}
					//return new Bomb(this, this->_player->getAttackList().at(territoryToBomb - 1));
				}
				else {
					cout << "You do not have a Bomb card." << endl;
				}
				return this;
			}

			if (orderType == 4)
			{
				if (this->_player->ownsCard("Blockade"))
				{
					cout << endl;
					cout << "What territory do you want to blockade?" << endl;
					int counter = 1;
					for (Territory* t : this->_player->getDefendList())
					{
						cout << counter << " - " << t->getName() << endl;
						counter++;
					}
					int territoryToBlock;
					cin >> territoryToBlock;
					while (territoryToBlock < 0 || territoryToBlock > this->_player->getDefendList().size())
					{
						cout << "Not a valid number." << endl;
						cout << endl;
						cout << "What territory do you want to block?" << endl;
						cin >> territoryToBlock;
					}
					//return new Blockade(this, this->_player->getAttackList().at(territoryToBlock - 1));
				}
				else 
				{
					cout << "You do not have a Blockade card." << endl;
				}
				return this;
			}

			if (orderType == 5)
			{
				if (this->_player->ownsCard("Airlift"))
				{
					cout << endl;
					cout << "What is the target territory to airlift?" << endl;
					int counter = 1;
					for (Territory* t : this->_player->getOwnedTerritories())
					{
						cout << counter << " - " << t->getName() << endl;
						counter++;
					}
					int airliftTarget;
					cin >> airliftTarget;
					while (airliftTarget < 0 || airliftTarget > this->_player->getOwnedTerritories().size())
					{
						cout << "Not a valid number." << endl;
						cout << endl;
						cout << "What is the target territory to airlift?" << endl;
						cin >> airliftTarget;
					}

					cout << endl;
					cout << "What is the source territory to airlift?" << endl;
					counter = 1;
					for (Territory* t : this->_player->getOwnedTerritories())
					{
						cout << counter << " - " << t->getName() << endl;
						counter++;
					}
					int airliftSource;
					cin >> airliftSource;
					while (airliftSource < 0 || airliftSource > this->_player->getOwnedTerritories().size())
					{
						cout << "Not a valid number." << endl;
						cout << endl;
						cout << "What is the source territory to airlift?" << endl;
						cin >> airliftSource;
					}

					int armiesToAirlift;
					cout << endl;
					cout << "How many armies to airlift?" << endl;
					cin >> armiesToAirlift;

					//return new Airlift(this, this->_player->getOwnedTerritories().at(airliftSource - 1),
					//	this->_player->getOwnedTerritories().at(airliftTarget - 1), armiesToAirlift);
				}
				else
				{
					cout << "You do not have a Airlift card." << endl;
				}
				return this;
			}

			if (orderType == 6)
			{
				if (this->_player->ownsCard("Diplomacy"))
				{
					cout << "Which player do you want to negociate with?" << endl;
					//TO DO
				}
				else
				{
					cout << "You do not have a Diplomacy card." << endl;
				}
				return this;
			}

			if (orderType == 0)
			{
				/*return NULL;*/
				return this;
			}
		}
	
    return this;
}

PlayerStrategy* HumanPlayerStrategy::toAttack()
{
	vector<Territory*> territoriesToAttack;
	cout << "Which neighbouring territories should be attacked in priority?" << endl;
	cout << endl;
	vector<Territory*> neighbouringTerritories = this->_player->getNeighbouringEnemyTerritories();
	int counter = 1;
	for (Territory* t : neighbouringTerritories)
	{
		cout << counter << " - " << t->getName() << endl;
		counter++;
	}
	cout << endl;
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
			cout << endl;
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
	this->_player->setAttackList(territoriesToAttack);
    return this;
}

PlayerStrategy* HumanPlayerStrategy::toDefend()
{
	vector<Territory*> territoriesToDefend;
	cout << "Which territories should be defended in priority?" << endl;
	int counter = 1;
	for (Territory* t : this->getPlayer()->getOwnedTerritories())
	{
		cout << counter << " - " << t->getName() << endl;
		counter++;
	}
	cout << endl;
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
			cout << endl;
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
	cout << endl;
	cout << "Prioritized list of territories to defend: " << endl;
	counter = 1;
	for (Territory* t : territoriesToDefend)
	{
		cout << counter << " - " << t->getName() << endl;
		counter++;
	}
	this->_player->setDefendList(territoriesToDefend);
    return this;
}

// Destructor

HumanPlayerStrategy::~HumanPlayerStrategy() = default;
