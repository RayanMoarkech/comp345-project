//
// COMP345_PROJECT_PLAYERSTRATEGIES_CPP PlayerStrategies.CPP
//

#include "../include/PlayerStrategies.h"
#include "../include/Player.h"
#include "../include/Map.h"
#include "../include/Orders.h"

#include <iostream>
    using std::cout;
	using std::cin;
    using std::endl;
#include <vector>
    using std::vector;
#include <algorithm>
	using std::sort;
#include <string>
	using std::string;

class Territory;
class Order;

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

// Defined method to be inherited by all strategies
void PlayerStrategy::playCard(string cardType)
{
	vector<Card*> playersCards = this->getPlayer()->getPlayerHand()->cards;
	
	for (int i = 0; i < playersCards.size(); i++)
	{
		cout << i << endl;
		if (playersCards.at(i)->getCardType() == cardType)
		{
			Card* card = playersCards.at(i);
			// return card to deck
			this->getPlayer()->getGameEngine()->getDeck()->returnCard(card);
			// remove card from hand
			this->getPlayer()->getPlayerHand()->
				cards.erase(this->getPlayer()->getPlayerHand()->cards.begin() + i);
			break;
		}
	}
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
Order* NeutralPlayerStrategy::issueOrder()
{
    return nullptr;
}

void NeutralPlayerStrategy::toAttack()
{
    // TODO: not sure what to put here
}

// is attacked, it becomes an Aggressive player
void NeutralPlayerStrategy::toDefend()
{
	//TODO
    //return new AggressivePlayerStrategy(*this);
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

Order* BenevolentPlayerStrategy::issueOrder()
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
		return nullptr;
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
		return new Deploy(this->getPlayer(), weakestCountry, armiesToDeploy);
	}

	if (this->getPlayer()->getArmyUnits() == 0)
	{
		//Only plays Blockade and Airlift cards (Defensive cards)
		if (this->getPlayer()->ownsCard("Blockade"))
		{
			Territory* weakestCountry = this->getPlayer()->getDefendList().at(0);
			cout << this->getPlayer()->getName() << " will play a Blockade card." << endl;
			cout << this->getPlayer()->getName() << " will blockade territory " << weakestCountry->getName() << endl;
			this->playCard("Blockade");
			return new Blockade(this->getPlayer(), weakestCountry);
		}
		if (this->getPlayer()->ownsCard("Airlift"))
		{
			Territory* weakestCountry = this->getPlayer()->getDefendList().front();
			Territory* strongestCountry = this->getPlayer()->getDefendList().back();
			int armiesToAirlift = strongestCountry->getNumberOfArmies() / 2;
			cout << this->getPlayer()->getName() << " will play a Airlift card." << endl;
			cout << this->getPlayer()->getName() << " will airlift " << armiesToAirlift << " armies to " 
				<< weakestCountry->getName() << " from " << strongestCountry->getName() << endl;
			this->playCard("Airlift");
			return new Airlift(this->getPlayer(), strongestCountry, weakestCountry, armiesToAirlift);
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
			return new Advance(this->getPlayer(), strongestCountry, weakestCountry, armiesToAdvance);
		}
	}
	cout << this->getPlayer()->getName() << " has no more orders to issue." << endl;
	return nullptr;
}

void BenevolentPlayerStrategy::toAttack()
{
	
}


void BenevolentPlayerStrategy::toDefend()
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

Order* AggressivePlayerStrategy::issueOrder() {
    //TODO: complete part
    return nullptr;
}
void AggressivePlayerStrategy::toAttack()
{
    //TODO: computer player that focuses on attack
    // deploys or advances armies on its strongest country,
    // then always advances to enemy territories until it cannot do so anymore
}

void AggressivePlayerStrategy::toDefend()
{
    //TODO: complete part
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

Order* HumanPlayerStrategy::issueOrder() {
	cout << endl;
	cout << "----------------------------------" << endl;
	cout << this->getPlayer()->getName() << "'s Turn - Type: Human" << endl;
	cout << "----------------------------------" << endl;
	cout << endl;

	//To Defend
	if (this->getPlayer()->getDefendList().size() == 0)
	{
		this->toDefend();
		return nullptr;
	}

	//To Attack
	if (this->getPlayer()->getAttackList().size() == 0)
	{
		this->toAttack();
		return nullptr;
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
			return new Deploy(this->getPlayer(), this->getPlayer()->getDefendList().at(territoryToDefend - 1), 
				armyUnitsToDeploy);
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
					return new Advance(this->getPlayer(), this->_player->getOwnedTerritories().at(sourceTerritory - 1),
						this->_player->getDefendList().at(territoryToDefend - 1), armiesToAdvance);
				}
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
				return new Advance(this->getPlayer(), this->_player->getOwnedTerritories().at(sourceTerritory - 1),
					this->_player->getAttackList().at(territoryToAttack - 1), armiesToAdvance);
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
					this->playCard("Bomb");
					return new Bomb(this->getPlayer(), this->_player->getAttackList().at(territoryToBomb - 1));
				}
				else {
					cout << endl;
					cout << "You do not have a Bomb card." << endl;
					return nullptr;
				}
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
					this->playCard("Blockade");
					return new Blockade(this->getPlayer(), this->_player->getAttackList().at(territoryToBlock - 1));
				}
				else 
				{
					cout << endl;
					cout << "You do not have a Blockade card." << endl;
					return nullptr;
				}
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

					this->playCard("Airlift");
					return new Airlift(this->getPlayer(), this->_player->getOwnedTerritories().at(airliftSource - 1),
					this->_player->getOwnedTerritories().at(airliftTarget - 1), armiesToAirlift);
				}
				else
				{
					cout << endl;
					cout << "You do not have a Airlift card." << endl;
					return nullptr;
				}
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
					cout << endl;
					cout << "You do not have a Diplomacy card." << endl;
					return nullptr;
				}
			}

			if (orderType == 0)
			{
				/*return NULL;*/
				return nullptr;
			}
		}
	
    return nullptr;
}

void HumanPlayerStrategy::toAttack()
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
}

void HumanPlayerStrategy::toDefend()
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
}

// Destructor

HumanPlayerStrategy::~HumanPlayerStrategy() = default;



// ---------------------------------------------
// -------CheaterPlayerStrategy Sectio-------
// ---------------------------------------------

// Constructors

CheaterPlayerStrategy::CheaterPlayerStrategy() : PlayerStrategy() {}

CheaterPlayerStrategy::CheaterPlayerStrategy(Player* player) : PlayerStrategy(player) {}

CheaterPlayerStrategy::CheaterPlayerStrategy(const PlayerStrategy& playerStrategy) : PlayerStrategy(playerStrategy) {}

// Functionalities

Order* CheaterPlayerStrategy::issueOrder()
{
  cout << endl;
  cout << "----------------------------------" << endl;
  cout << this->getPlayer()->getName() << "'s Turn - Type: Cheater" << endl;
  cout << "----------------------------------" << endl;
  cout << endl;

  //To Defend
  if (this->isAttackedOncePerTurn() == true)
  {
    cout <<"Cheater has already attacked before this turn!." << endl;
    return nullptr;
  }

  vector<Territory*> territoriesToAttack = this->getPlayer()->getNeighbouringEnemyTerritories();
  for (Territory* territory : territoriesToAttack)
  {
   territory->setOwnedBy(this->getPlayer(),territory->getNumberOfArmies() );
   cout << territory->getName()<< "is now owned By the Cheater player" << endl;
  }
  this->setAttackedOncePerTurn(true);
  return nullptr;
}

void CheaterPlayerStrategy::toAttack()
{
  // Cheater player automatically conquers all adjacent territories.
}


void CheaterPlayerStrategy::toDefend()
{
  // Cheater player does not defend.
}
bool CheaterPlayerStrategy::isAttackedOncePerTurn() const {
  return attackedOncePerTurn;
}
void CheaterPlayerStrategy::setAttackedOncePerTurn(bool attackedOncePerTurn) {
  CheaterPlayerStrategy::attackedOncePerTurn = attackedOncePerTurn;
}

// Destructor

CheaterPlayerStrategy::~CheaterPlayerStrategy() = default;


