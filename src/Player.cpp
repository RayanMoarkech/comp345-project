//
// COMP345_PROJECT_PLAYER_CPP Player.cpp
//

#include <cstdlib>     /* srand, rand */
#include <vector>
    using std::vector;
#include <iostream>
    using std::cout;
    using std::endl;

#include "../include/Player.h"


Player::Player() {
// TO DO
}

Player::Player(vector<Territory*> ownedTerritories, Deck* playerDeck, OrdersList* playerOrders)
{
	this->ownedTerritories = ownedTerritories;
	this->playerDeck = playerDeck;
	this->playerOrders = playerOrders;
}

Player::Player(const Player& player) {
	this->ownedTerritories = player.ownedTerritories;
	this->playerDeck = player.playerDeck;
	this->playerOrders = player.playerOrders;
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

	return territoriesToAttack;
}

void Player::issueOrder()
{
	//TO DO 
	// Create new order
	//Order newOrder = new Order();
	// Add it to the player's orders list
	//this->playerOrders.addOrder(newOrder);
}


