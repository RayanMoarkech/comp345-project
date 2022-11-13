//
// COMP345_PROJECT_PLAYER_CPP Player.cpp
//

#include <cstdlib>     /* srand, rand */
#include <utility>
#include <vector>
    using std::vector;
#include <iostream>
    using std::cout;
	using std::cin;
    using std::endl;
#include <ostream>
#include <algorithm>

#include "../include/Player.h"
#include "../include/Orders.h"
#include "../include/Cards.h"

int Player::idCounter = 1;

//Default constructor
Player::Player() {
    this->id = idCounter++;
    this->name = "Player " + std::to_string(this->id);
	this->ownedTerritories = {};
	this->playerHand  = new Hand();
	this->playerOrders = new OrdersList();
    this->armyUnits = 0;
}

//Copy constructor (Deep)
Player::Player(const Player& player) {
	//Deep copy all territories
    this->id = idCounter++;
    this->name = player.name;
	this->ownedTerritories = {};
	for (Territory* t : player.ownedTerritories)
	{
		Continent* newContinent = new Continent(*t->getContinent());
		Territory* newTerritory = new Territory(*t, newContinent);
		this->ownedTerritories.push_back(newTerritory);
	}
	this->playerHand = new Hand(*player.playerHand);
	this->playerOrders = new OrdersList(*player.playerOrders);
    this->armyUnits = player.armyUnits;
}

//Parametrized constructor
Player::Player(string name, vector<Territory*> ownedTerritories, Hand* playerHand, OrdersList* playerOrders)
{
    this->id = idCounter++;
    this->name = std::move(name);
	this->ownedTerritories = ownedTerritories;
	this->playerHand = playerHand;
	this->playerOrders = playerOrders;
    this->armyUnits = 0;
}

//Destructor
Player::~Player()
{
	for (Territory* t : this->ownedTerritories)
	{
		delete t;
		t = nullptr;
	}
	delete this->playerHand;
	this->playerHand = nullptr;
	delete this->playerOrders;
	this->playerOrders = nullptr;
}

//Assignment Operator
Player& Player::operator=(const Player& player)
{
    this->id = player.id;
	this->ownedTerritories = player.ownedTerritories;
	this->playerHand = player.playerHand;
	this->playerOrders = player.playerOrders;
    this->armyUnits = player.armyUnits;
	return *this;
}

string Player::getName()
{
    return this->name;
}

void Player::setName(string name)
{
	this->name = name;
}

void Player::setArmyUnits(int armyUnits)
{
    this->armyUnits = armyUnits;
}

int Player::getArmyUnits()
{
    return this->armyUnits;
}

//Stream Insertion Operator for Player class
ostream& operator<<(ostream& os, const Player& player)
{
    os << "Player " << player.name << " with id " << player.id << "." << endl;
	vector<Territory*> ownedTerritories = player.ownedTerritories;
	os << "Player owns territories: " << endl;
	//print all territory names
	for (Territory* t : player.ownedTerritories)
	{
		os << t->getName() << endl;
	}
	//Orders and Orderslist do not have a name, so using size instead
	os << "Player has issued " << player.playerOrders->getOrdersList().size() << " orders." << endl;
	os << "Player has following cards in hand" << endl;
	for (Card* c : player.playerHand->cards)
	{
		os << c->getCardType() << endl;
	}
	return os;
}

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

vector<Territory*> Player::toDefend()
{
	vector<Territory*> territoriesToDefend;
	cout << "Which territories should be defended in priority?" << endl;
	int counter = 1;
	for (Territory* t : this->ownedTerritories)
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
		else if (territoryToDefend < 0 || territoryToDefend > this->ownedTerritories.size())
		{
			cout << "Not a valid number." << endl;
			cout << "Enter number corresponding to territory to add to list. Enter 0 to stop." << endl;
		}
		else
		{
			if (!isIn(territoriesToDefend, this->ownedTerritories.at(territoryToDefend - 1)))
			{
				territoriesToDefend.push_back(this->ownedTerritories.at(territoryToDefend - 1));
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
	return territoriesToDefend;
}

vector<Territory*> Player::toAttack()
{
	vector<Territory*> territoriesToAttack;
	cout << "Which neighbouring territories should be attacked in priority?" << endl;
	vector<Territory*> neighbouringTerritories = this->getNeighbouringTerritories();
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
	return territoriesToAttack;
}

vector<Territory*> Player::getNeighbouringTerritories()
{
	vector<Territory*> neighbouringTerritories;
	for (Territory* t : this->ownedTerritories)
	{
		for (Territory* adjTerritory : t->getAdjacentTerritories())
		{
			if (!isIn(neighbouringTerritories, adjTerritory) && !isIn(ownedTerritories, adjTerritory)) 
			{
				neighbouringTerritories.push_back(adjTerritory);
			}
		}
	}
	return neighbouringTerritories;
}

Order* Player::issueOrder(string orderType)
{
	cout << "----------------------------------" << endl;
	cout << this->name << "'s Turn" << endl;
	cout << "----------------------------------" << endl;

	//To Attack
	//vector<Territory*> toAttack = this->toAttack();
	if (orderType == "toAttack")
	{
		this->toAttackList = this->toAttack();
		return NULL;
	}
	//To Defend
	//vector<Territory*> toDefend = this->toDefend();
	if (orderType == "toDefend")
	{
		this->toDefendList = this->toDefend();
		return NULL;
	}

	//Deploy on toDefend territories
	if (orderType == "Deploy")
	{
		cout << "Choose territory and how many many army units to deploy." << endl;
		int counter = 1;
		for (Territory* t : this->toDefendList)
		{
			cout << counter << " - " << t->getName() << endl;
			counter++;
		}
		int territoryToDefend = -1;
		int armyUnitsToDeploy;

		cout << "Total remaining of army units to deploy: " << this->armyUnits << endl;
		cout << "Choose territory: ";
		cin >> territoryToDefend;

		if (territoryToDefend <= 0 || territoryToDefend > this->toDefendList.size())
		{
			cout << "Not a valid number." << endl;
			cout << "Enter number corresponding to territory to add to list. Enter 0 to stop." << endl;
		}
		else
		{
			cout << "Number of army units to deploy to " << this->toDefendList.at(territoryToDefend - 1)->getName() << ": ";
			cin >> armyUnitsToDeploy;
			while (armyUnitsToDeploy > this->armyUnits || armyUnitsToDeploy <= 0)
			{
				cout << "Not a valid number of army units" << endl;
				cout << "Number of army units to deploy to " << this->toDefendList.at(territoryToDefend - 1)->getName() << ": ";
				cin >> armyUnitsToDeploy;
			}
			this->armyUnits -= armyUnitsToDeploy;
			return new Deploy(this, this->toDefendList.at(territoryToDefend - 1), armyUnitsToDeploy);
		}
	}

	//List of the rest of kind of orders
	if (orderType == "AdvanceOrPlayCard")
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
			cout << "What territory do you want to defend?" << endl;
			int counter = 1;
			for (Territory* t : this->toDefendList)
			{
				cout << counter << " - " << t->getName() << endl;
				counter++;
			}
			int territoryToDefend;
			cin >> territoryToDefend;
			while (territoryToDefend < 0 || territoryToDefend > this->toDefendList.size())
			{
				cout << "Not a valid number." << endl;
				cout << "What territory do you want to defend?" << endl;
				cin >> territoryToDefend;
			}
			cout << "What territory is the source of armies?" << endl;
			counter = 1;
			for (Territory* t : this->ownedTerritories)
			{
				cout << counter << " - " << t->getName() << endl;
				counter++;
			}
			int sourceTerritory;
			cin >> sourceTerritory;
			while (sourceTerritory < 0 || sourceTerritory > this->ownedTerritories.size())
			{
				cout << "Not a valid number." << endl;
				cout << "What territory is the source of armies?" << endl;
				cin >> sourceTerritory;
			}
			int armiesToAdvance;
			cout << "How many armies to advance?" << endl;
			cin >> armiesToAdvance;
			return new Advance(this, this->ownedTerritories.at(sourceTerritory - 1),
				this->toDefendList.at(territoryToDefend - 1), armiesToAdvance);
		}

		if (orderType == 2)
		{
			cout << "What territory do you want to attack?" << endl;
			int counter = 1;
			for (Territory* t : this->toAttackList)
			{
				cout << counter << " - " << t->getName() << endl;
				counter++;
			}
			int territoryToAttack;
			cin >> territoryToAttack;
			while (territoryToAttack < 0 || territoryToAttack > this->toAttackList.size())
			{
				cout << "Not a valid number." << endl;
				cout << "What territory do you want to attack?" << endl;
				cin >> territoryToAttack;
			}
			cout << "What territory is the source of armies?" << endl;
			counter = 1;
			for (Territory* t : this->ownedTerritories)
			{
				cout << counter << " - " << t->getName() << endl;
				counter++;
			}
			int sourceTerritory;
			cin >> sourceTerritory;
			while (sourceTerritory < 0 || sourceTerritory > this->ownedTerritories.size())
			{
				cout << "Not a valid number." << endl;
				cout << "What territory is the source of armies?" << endl;
				cin >> sourceTerritory;
			}
			int armiesToAdvance;
			cout << "How many armies to advance?" << endl;
			cin >> armiesToAdvance;
			return new Advance(this, this->ownedTerritories.at(sourceTerritory - 1),
				this->toAttackList.at(territoryToAttack - 1), armiesToAdvance);
		}

		if (orderType == 3)
		{
			cout << "What territory do you want to bomb?" << endl;
			int counter = 1;
			for (Territory* t : this->toAttackList)
			{
				cout << counter << " - " << t->getName() << endl;
				counter++;
			}
			int territoryToBomb;
			cin >> territoryToBomb;
			while (territoryToBomb < 0 || territoryToBomb > this->toAttackList.size())
			{
				cout << "Not a valid number." << endl;
				cout << "What territory do you want to bomb?" << endl;
				cin >> territoryToBomb;
			}
			return new Bomb(this, this->toAttackList.at(territoryToBomb - 1));
		}

		if (orderType == 4)
		{
			cout << "What territory do you want to block?" << endl;
			int counter = 1;
			for (Territory* t : this->toAttackList)
			{
				cout << counter << " - " << t->getName() << endl;
				counter++;
			}
			int territoryToBlock;
			cin >> territoryToBlock;
			while (territoryToBlock < 0 || territoryToBlock > this->toAttackList.size())
			{
				cout << "Not a valid number." << endl;
				cout << "What territory do you want to block?" << endl;
				cin >> territoryToBlock;
			}
			return new Bomb(this, this->toAttackList.at(territoryToBlock - 1));
		}

		if (orderType == 5)
		{
			cout << "What is the target territory to airlift?" << endl;
			int counter = 1;
			for (Territory* t : this->ownedTerritories)
			{
				cout << counter << " - " << t->getName() << endl;
				counter++;
			}
			int airliftTarget;
			cin >> airliftTarget;
			while (airliftTarget < 0 || airliftTarget > this->ownedTerritories.size())
			{
				cout << "Not a valid number." << endl;
				cout << "What is the target territory to airlift?" << endl;
				cin >> airliftTarget;
			}

			cout << "What is the source territory to airlift?" << endl;
			counter = 1;
			for (Territory* t : this->ownedTerritories)
			{
				cout << counter << " - " << t->getName() << endl;
				counter++;
			}
			int airliftSource;
			cin >> airliftSource;
			while (airliftSource < 0 || airliftSource > this->ownedTerritories.size())
			{
				cout << "Not a valid number." << endl;
				cout << "What is the source territory to airlift?" << endl;
				cin >> airliftSource;
			}

			int armiesToAirlift;
			cout << "How many armies to airlift?" << endl;
			cin >> armiesToAirlift;

			return new Airlift(this, this->ownedTerritories.at(airliftSource - 1),
				this->ownedTerritories.at(airliftTarget - 1), armiesToAirlift);
		}

		if (orderType == 6)
		{
			//To Do negotiation
		}

		if (orderType == 0)
		{
			return NULL;
		}
	}
}

vector<Territory*> Player::getOwnedTerritories()
{
	return this->ownedTerritories;
}

Hand* Player::getPlayerHand()
{
	return this->playerHand;
}

OrdersList* Player::getPlayerOrders()
{
	return this->playerOrders;
}

void Player::addOwnedTerritory(Territory *territory)
{
    this->ownedTerritories.push_back(territory);
}

