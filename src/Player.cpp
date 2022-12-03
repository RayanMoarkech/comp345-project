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
#include "../include/PlayerStrategies.h"

int Player::idCounter = 1;

//Default constructor
Player::Player() {
    this->id = idCounter++;
    this->name = "Player " + std::to_string(this->id);
	this->ownedTerritories = {};
	this->playerHand  = new Hand();
	this->playerOrders = new OrdersList();
	this->isNeutral = false;
	this->conqueredTerritory = false;
    this->armyUnits = 0;
	this->ps = new HumanPlayerStrategy(); //default player is human
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
	this->negotiatingWith = std::list<Player*>(player.negotiatingWith);
	this->isNeutral = player.isNeutral;
	this->conqueredTerritory = player.conqueredTerritory;
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
	this->isNeutral = false;
	this->conqueredTerritory = false;
    this->armyUnits = 0;
}

Player::Player(bool isNeutral)
{
	this->ownedTerritories = {};
	this->playerHand = new Hand();
	this->playerOrders = new OrdersList();
	this->isNeutral = isNeutral;
	this->conqueredTerritory = false;
    this->armyUnits = 0;
	this->name = "Neutral Player";
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
	this->ownedTerritories = ownedTerritories;
	this->playerHand = playerHand;
	this->playerOrders = playerOrders;
	this->negotiatingWith = negotiatingWith;
	this->isNeutral = isNeutral;
	this->conqueredTerritory = conqueredTerritory;
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

void Player::setAttackList(vector<Territory*> toAttackList)
{
	this->toAttackList = toAttackList;
}

vector<Territory*> Player::getDefendList()
{
	return this->toDefendList;
}

void Player::setDefendList(vector<Territory*> toDefendList)
{
	this->toDefendList = toDefendList;
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

	vector<Territory*> territoriesToAttack = this->getNeighbouringEnemyTerritories();
	std::random_shuffle(territoriesToAttack.begin(), territoriesToAttack.end());
	for (Territory* t : territoriesToAttack)
	{
		cout << t->getName() << endl;
	}
	this->toAttackList = territoriesToAttack;
}

//This method gets all adjacent territories of owned territories and excludes territories that are owned by player
vector<Territory*> Player::getNeighbouringEnemyTerritories()
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

//Gets enemy territories adjacent to 1 territory
vector<Territory*> Player::getNeighbouringEnemyTerritories(Territory* t)
{
	vector<Territory*> neighbouringTerritories;
	for (Territory* adjTerritory : t->getAdjacentTerritories())
	{
		if (!isIn(neighbouringTerritories, adjTerritory) && !isIn(ownedTerritories, adjTerritory))
		{
				neighbouringTerritories.push_back(adjTerritory);
		}
	}
	return neighbouringTerritories;
}

//Gets owned territories adjacent to 1 territory
vector<Territory*> Player::getNeighbouringOwnedTerritories(Territory* t)
{
	vector<Territory*> neighbouringTerritories;
	for (Territory* adjTerritory : t->getAdjacentTerritories())
	{
		if (!isIn(neighbouringTerritories, adjTerritory) && isIn(ownedTerritories, adjTerritory))
		{
			neighbouringTerritories.push_back(adjTerritory);
		}
	}
	return neighbouringTerritories;
}

Order* Player::issueOrder()
{
	return this->ps->issueOrder();
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

void Player::ownTerritory(Territory *territory, int armyUnits)
{
    this->armyUnits = this->getArmyUnits() - armyUnits;
    this->addOwnedTerritory(territory);
}

void Player::addTerritory(Territory* t)
{
	this->ownedTerritories.push_back(t);
}

void Player::addNegotiator(Player* p)
{
	negotiatingWith.push_back(p);
}

std::list<Player*> Player::getNegotiatorList()
{
	return this->negotiatingWith;
}

void Player::setConqueredTerritory(bool conqueredTerritory)
{
	this->conqueredTerritory = conqueredTerritory;
}

bool Player::getConqueredTerritory()
{
	return conqueredTerritory;
}

void Player::addOwnedTerritory(Territory *territory)
{
    this->ownedTerritories.push_back(territory);
}

void Player::removeTerritory(Territory* t)
{
	this->ownedTerritories.erase(std::remove(this->ownedTerritories.begin(), this->ownedTerritories.end(), t), this->ownedTerritories.end());
}

void Player::removeReinforcements(int numArmies)
{
	this->armyUnits = this->armyUnits - numArmies;
}

bool Player::ownsCard(string cardType)
{
	for (Card* c : this->playerHand->cards)
	{
		if (c->getCardType() == cardType)
		{
			return true;
		}
	}
	return false;
}

PlayerStrategy* Player::getPlayerStrategy()
{
	return this->ps;
}

void Player::setPlayerStrategy(PlayerStrategy* ps)
{
	this->ps = ps;
}

GameEngine* Player::getGameEngine()
{
	return this->gameEngine;
}

void Player::setGameEngine(GameEngine* gameEngine)
{
	this->gameEngine = gameEngine;
}