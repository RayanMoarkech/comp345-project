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

vector<Territory*> Player::getAttackList()
{
	return this->toAttackList;
}

vector<Territory*> Player::getDefendList()
{
	return this->toDefendList;
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

//returns owned territories in random order
void Player::toDefend()
{
	cout << endl;
	cout << this->getName() << " 's Territories to Defend in Prioritized Order" << endl;
	cout << endl;

	vector<Territory*> territoriesToDefend = this->ownedTerritories;
	std::random_shuffle(territoriesToDefend.begin(), territoriesToDefend.end());
	for (Territory* t : territoriesToDefend)
	{
		cout << t->getName() << endl;
	}
	this->toDefendList = territoriesToDefend;
}

//returns adjacent territories not belong to player in random order
void Player::toAttack()
{
	cout << endl;
	cout << this->getName() << " 's Territories to Attack in Prioritized Order" << endl;
	cout << endl;

	vector<Territory*> territoriesToAttack = this->getNeighbouringTerritories();
	std::random_shuffle(territoriesToAttack.begin(), territoriesToAttack.end());
	for (Territory* t : territoriesToAttack)
	{
		cout << t->getName() << endl;
	}
	this->toAttackList = territoriesToAttack;
}

//This method gets all adjacent territories of owned territories and excludes territories that are owned by player
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

Order* Player::issueOrder()
{
	cout << "----------------------------------" << endl;
	cout << this->name << "'s Turn" << endl;
	cout << "----------------------------------" << endl;

	//To Attack
	if (this->toAttackList.size() == 0)
	{
		this->toAttack();
		return NULL;
	}
	//To Defend
	if (this->toDefendList.size() == 0)
	{
		this->toDefend();
		return NULL;
	}

	//Deploy on toDefend territories, will not be able to issue other orders until
	//all armies are deployed
	if (this->armyUnits != 0)
	{
		int index = -1;
		for (Territory* t : this->toDefendList)
		{
			index++;
			if (t->getNumberOfArmies() == 0)
			{
				break;
			}
		}
		srand(time(0));
		int armiesToDeploy = 1 + (rand() % this->armyUnits);
		cout << endl;
		cout << this->getName() << " deploys " << armiesToDeploy << " to " << this->toDefendList.at(index)->getName() << endl;
		this->armyUnits -= armiesToDeploy;
		cout << this->getName() << " has " << this->armyUnits << " armies remaining to deploy." << endl;
		cout << endl;
		return new Deploy(this, this->toDefendList.at(index), armiesToDeploy);
	}

	//Advance (Defend)
	if (this->toDefendList.size() != 5)
	{
		Territory* targetTerritory = this->toDefendList.at(0);
		this->toDefendList.erase(this->toDefendList.begin());
		int index = (rand() % this->toDefendList.size());
		Territory *sourceTerritory = this->toDefendList.at(index);
		int armiesToAdvance = sourceTerritory->getNumberOfArmies();


		cout << endl;
		cout << "Defense: " << this->getName() << " advances " << armiesToAdvance << " armies from " <<
			sourceTerritory->getName() << " to " << targetTerritory->getName() << endl;
		cout << endl;
		return new Advance(this, sourceTerritory,targetTerritory, armiesToAdvance);
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
	return NULL;
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

