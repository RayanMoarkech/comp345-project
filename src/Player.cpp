//
// COMP345_PROJECT_PLAYER_CPP Player.cpp
//

#include <cstdlib>     /* srand, rand */
#include <vector>
    using std::vector;
#include <iostream>
    using std::cout;
    using std::endl;
#include <ostream>

#include "../include/Player.h"
#include "../include/Orders.h"
#include "../include/Cards.h"

//Default constructor
Player::Player() {
	this->ownedTerritories = {};
	this->playerHand  = new Hand();
	this->playerOrders = new OrdersList();
}

//Copy constructor (Deep)
Player::Player(const Player& player) {
	//Deep copy all territories
	this->ownedTerritories = {};
	for (Territory* t : player.ownedTerritories)
	{
		Continent* newContinent = new Continent(*t->getContinent());
		Territory* newTerritory = new Territory(*t, newContinent);
		this->ownedTerritories.push_back(newTerritory);
	}
	this->playerHand = new Hand(player.playerHand);
	this->playerOrders = new OrdersList(*player.playerOrders);
}

//Parametrized constructor
Player::Player(vector<Territory*> ownedTerritories, Hand* playerHand, OrdersList* playerOrders)
{
	this->ownedTerritories = ownedTerritories;
	this->playerHand = playerHand;
	this->playerOrders = playerOrders;
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
	return *this;
}

//Stream Insertion Operator for Player class
ostream& operator<<(ostream& os, const Player& player)
{
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
	for (Card* c : player.playerHand.cards)
	{
		os << c.getType(); << endl
	}
	return os;
}

vector<Territory*> Player::toDefend()
{
	//Load map
	Map *map = MapLoader::load("./001_I72_Ghtroc720.map");

	//Get number of total territories in the map
	int nbOfTerritories = map->getTerritories().size();
	cout << "Number Of Total Territories in Map: " << nbOfTerritories << endl;

	//Get arbitrary and random number of territories to defend
	int nbOfTerritoriesToDefend = rand() % nbOfTerritories;
	cout << "Arbitrary Number Of Territories To Defend: " << nbOfTerritoriesToDefend << endl;

	//Empty vector of Territory pointers
	vector<Territory*> territoriesToDefend;

	//Add random territory from map to the territories to defend vector
	for (int i = 0; i < nbOfTerritoriesToDefend; i++) {
		territoriesToDefend.push_back(map->getTerritories().at(rand() % nbOfTerritories));
	}

	//Print the territory names to console as part of the test
	for (int i = 0; i < nbOfTerritoriesToDefend; i++) {
		cout << territoriesToDefend.at(i)->getName() << endl;
	}

	delete map;
	map = nullptr;

	return territoriesToDefend;
}

vector<Territory*> Player::toAttack()
{

	//Load map
	Map *map = MapLoader::load("./001_I72_Ghtroc720.map");

	//Get number of total territories in the map
	int nbOfTerritories = map->getTerritories().size();
	cout << "Number Of Total Territories in Map: " << nbOfTerritories << endl;

	//Get arbitrary and random number of territories to attack
	int nbOfTerritoriesToAttack = rand() % nbOfTerritories;
	cout << "Arbitrary Number Of Territories To Attack: " << nbOfTerritoriesToAttack << endl;

	//Empty vector of Territory pointers
	vector<Territory*> territoriesToAttack;

	//Add random territory from map to the territories to attack vector
	for (int i = 0; i < nbOfTerritoriesToAttack; i++) {
		territoriesToAttack.push_back(map->getTerritories().at(rand() % nbOfTerritories));
	}

	//Print the territory names to console as part of the test
	for (int i = 0; i < nbOfTerritoriesToAttack; i++) {
		cout << territoriesToAttack.at(i)->getName() << endl;
	}

	delete map;
	map = nullptr;

	return territoriesToAttack;
}

void Player::issueOrder()
{
	// Create new order
	Order* newOrder = new Order();
	// Add it to the player's orders list
	this->playerOrders->addOrder(newOrder);
	cout << "New Order Issued!" << endl;
	cout << "playerOrders is now of size " << this->playerOrders->getOrdersList().size() << endl;
}


