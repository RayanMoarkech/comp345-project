//
// COMP345_PROJECT_ORDERS_CPP Orders.cpp
//

#include "../include/Orders.h"
#include "../include/Map.h"
#include "../include/Player.h"
#include <ostream>

//----Order Class----

// Default constructor for Order parent class
Order::Order()
{
	executed = false;
	player = nullptr;
}

//Parametrized constructor for Order parent class
Order::Order(Player* player)
{
	executed = false;
	this->player = player;
}

//Destructor for Order parent class
Order::~Order()
{
	this->player = nullptr;
}

//Copy constructor for Order parent class
Order::Order(const Order& toCopy)
{
	this->player = toCopy.player; //Player of order will point to the same player as toCopy
	executed = toCopy.executed;
}

//Assignment operator for Order parent class
Order& Order::operator=(const Order& o)
{
	this->player = o.player;
	executed = o.executed;
	return *this;
}

//Stream insertion operator method for Order parent class
ostream& operator << (ostream& out, const Order& toOutput)
{
	return toOutput.print(out);
}

//Virtual method for derived classes to override print
ostream& Order::print(ostream& out) const
{
	out << "This is an order.";
	if (executed) {
		out << " This order was executed. The effect was order effect." << endl;
	}
	return out;
}

//Method to access the player associated with an order
Player* Order::getPlayer()
{
	return player;
}

//----Deploy Class----
//Default constructor for Deploy class
Deploy::Deploy() : Order()
{
	targetTerritory = nullptr;
	numOfArmyUnits = 0;
	cout << "Created a Deploy order." << endl;
}

//Parametrized constructor for Deploy class
Deploy::Deploy(Player* player, Territory* territory, unsigned int numOfArmyUnits) : Order(player)
{
	this->targetTerritory = territory;
	this->numOfArmyUnits = numOfArmyUnits;
}

//Destructor for deploy class
Deploy::~Deploy()
{
	this->targetTerritory = nullptr;
}

//Copy constructor for Deploy class
Deploy::Deploy(const Deploy& toCopy) : Order(toCopy)
{
	this->targetTerritory = toCopy.targetTerritory;
	this->numOfArmyUnits = toCopy.numOfArmyUnits;
}

//Assignment operator for Deploy class
Deploy& Deploy::operator=(const Deploy& rightSide)
{
	Order::operator=(rightSide);
	this->targetTerritory = rightSide.targetTerritory;
	this->numOfArmyUnits = rightSide.numOfArmyUnits;
	return *this;
}

//Stream insertion operator for deploy class
ostream& operator<<(ostream& out, const Deploy& toOutput)
{
	return toOutput.print(out);
}

ostream& Deploy::print(ostream& out) const
{
	out << "This is a Deploy order. ";
	if (executed) {
		out << " This order was executed. The effect was Deploy effect." << endl;
		return out;
	}
	return out;
}

//Method to return whether deploy order is valid
bool Deploy::validate()
{
	// Check that targetTerritory belongs to player issuing order
	return(targetTerritory->getOwnedBy() == this->getPlayer());
}

//Method to validate and execute deploy order
void Deploy::execute()
{
	if (!validate()) {
		cout << "Deploy order invalid. Will not execute." << endl;
		return;
	}
	// Remove numOfArmyUnits from reinforcement pool
	// Add numOfArmyUnits to targetTerritory

	player->ownTerritory(targetTerritory, numOfArmyUnits);

	targetTerritory->addArmies(numOfArmyUnits);
	cout << "Deploy order executed. Deployed " << numOfArmyUnits << " armies to " << targetTerritory->getName() << "." << endl;
	executed = true;
    notify();
}

std::string Deploy::stringToLog() {
    return "ORDER::Execute(): Deploy order executed.";
}

//----Advance Class----
//Deafult constructor for advance class
Advance::Advance() : Order()
{
	this->targetTerritory = nullptr;
	this->sourceTerritory = nullptr;
	numOfArmyUnits = 0;
	cout << "Created an Advance order." << endl;
}

//Parametized constructor for Advance class
Advance::Advance(Player* player, Territory* source, Territory* target, unsigned int numOfArmyUnits) : Order(player)
{
	this->sourceTerritory = source;
	this->targetTerritory = target;
	this->numOfArmyUnits = numOfArmyUnits;
}

//Destructor for Advance class
Advance::~Advance()
{
	this->sourceTerritory = nullptr;
	this->targetTerritory = nullptr;
}

//Copy constructor for Advance class
Advance::Advance(const Advance& toCopy) : Order(toCopy)
{
	this->sourceTerritory = toCopy.sourceTerritory;
	this->targetTerritory = toCopy.targetTerritory;
	this->numOfArmyUnits = toCopy.numOfArmyUnits;
}

//Assignment operator for Advance class
Advance& Advance::operator=(const Advance& rightSide)
{
	Order::operator=(rightSide);
	sourceTerritory = rightSide.sourceTerritory;
	targetTerritory = rightSide.targetTerritory;
	numOfArmyUnits = rightSide.numOfArmyUnits;
	return *this;
}

//Stream insertion operator for Advance class
ostream& operator<<(ostream& out, const Advance& toOutput)
{
	return toOutput.print(out);
}

ostream& Advance::print(ostream& out) const
{
	out << "This is an Advance order.";
	if (executed) {
		out << " This order was executed. The effect was Advance effect.";
	}
	return out;
}

//Validate method for Advance class
bool Advance::validate()
{
	// Check if source territory belongs to player and that territories are adjacent
	//Check if players are in negotiation
	vector<Territory*> adjacent = sourceTerritory->getAdjacentTerritories();
	std::list<Player*> negotiating = this->getPlayer()->getNegotiatorList();
	return((sourceTerritory->getOwnedBy() == this->getPlayer()) && (std::find(adjacent.begin(), adjacent.end(), targetTerritory) != adjacent.end()) && std::find(negotiating.begin(), negotiating.end(), targetTerritory->getOwnedBy()) == negotiating.end());
}

//Execute method for Advance class
void Advance::execute()
{
	if (!validate()) {
		cout << "Advance order invalid. Will not execute" << endl;
		return;
	}
	if (targetTerritory->getOwnedBy() == this->getPlayer()) {
		sourceTerritory->removeArmies(numOfArmyUnits);
		targetTerritory->addArmies(numOfArmyUnits);
		cout << "Defensive Advance Order Executed. Player advanced " << numOfArmyUnits << " armies from " << sourceTerritory->getName() << " to " << targetTerritory->getName() << "." << endl;
	}
	else {
		while (numOfArmyUnits > 0 && targetTerritory->getNumberOfArmies() > 0) {
			srand(time(nullptr));
			if (rand() % 10 < 6)
				targetTerritory->removeArmies(1);
			else if (rand() % 10 < 7)
				sourceTerritory->removeArmies(1);
				numOfArmyUnits--;
		}
		if (targetTerritory->getNumberOfArmies() == 0) {//all defender units elimated
			cout << "Previous owner of " << targetTerritory->getName() << ": " << targetTerritory->getOwnedBy()->getName() << endl;
			targetTerritory->getOwnedBy()->removeTerritory(targetTerritory); //Remove territory from defender
			sourceTerritory->getOwnedBy()->addTerritory(targetTerritory); //Add territory to attacker
			targetTerritory->setOwnedBy(sourceTerritory->getOwnedBy(), numOfArmyUnits); //Set territory owner and num armies to attacker
			cout << "Attacking Advance Order Executed. Attacking player won " << targetTerritory->getName() << " territory." << endl;
			cout << "New owner of " << targetTerritory->getName() << ": " << targetTerritory->getOwnedBy()->getName() << endl;
			sourceTerritory->getOwnedBy()->setConqueredTerritory(true);
		}
		else {//attacking player loses
			cout << "Attacking Advance Order Executed. Attacking player lost " << numOfArmyUnits << " armies and now has " << sourceTerritory->getNumberOfArmies() << " armies on " << sourceTerritory->getName() << " territory." << endl;
		}
	}
	executed = true;
    notify();
}

std::string Advance::stringToLog() {
    return "ORDER::Execute(): Advance order executed.";
}

//----Bomb Class----

//Default constructor for Bomb class
Bomb::Bomb() : Order() {
	targetTerritory = nullptr;
	cout << "Created a Bomb order." << endl;
}

//Parametized constructor for Bomb class
Bomb::Bomb(Player* player, Territory* target) : Order(player)
{
	this->targetTerritory = target;
}

//Destructor for Bomb class
Bomb::~Bomb()
{
	this->targetTerritory = nullptr;
}

//Copy constructor for Bomb class
Bomb::Bomb(const Bomb& toCopy) : Order(toCopy)
{
	this->targetTerritory = toCopy.targetTerritory;
}

//Assignment operator for Bomb class
Bomb& Bomb::operator=(const Bomb& rightSide)
{
	Order::operator=(rightSide);
	targetTerritory = rightSide.targetTerritory;
	return *this;
}

//Stream insertion operator for Bomb class
ostream& operator<<(ostream& out, const Bomb& toOutput)
{
	return toOutput.print(out);
}

ostream& Bomb::print(ostream& out) const
{
	out << "This is a Bomb order.";
	if (executed) {
		out << " This order was executed. The effect was Bomb effect.";
	}
	return out;
}

//Validate method for Bomb class
bool Bomb::validate()
{
	//TODO: Check that player has bomb card
	//How to check that bomb card was played??
	// 
	//Check that target territory is adjacent to one of player's current territories
	Player* player = this->getPlayer();
	vector<Territory*> playerTerritories = player->getOwnedTerritories();
	std::list<Player*> negotiating = this->getPlayer()->getNegotiatorList();
	bool isAdjacent = false;
	for (auto& territory : playerTerritories) {
		vector<Territory*> adjacent = territory->getAdjacentTerritories();
		isAdjacent = std::find(adjacent.begin(), adjacent.end(), targetTerritory) != adjacent.end();
		if (isAdjacent) break;
	}

	//Check that target territory does not belong to player as well and players not in negotiation
	return(targetTerritory->getOwnedBy() != this->getPlayer() && isAdjacent && std::find(negotiating.begin(), negotiating.end(), targetTerritory->getOwnedBy()) == negotiating.end());
}

void Bomb::execute()
{
	if (!validate()) {
		cout << "Bomb order invalid. Will not execute" << endl;
		return;
	}
	targetTerritory->setNumberOfArmies(targetTerritory->getNumberOfArmies() / 2);
	executed = true;
	cout << "Bomb order executed. Half of the army units removed from territory" << targetTerritory->getName() << ". " << endl;
    notify();
}

std::string Bomb::stringToLog() {
    return "ORDER::Execute(): Bomb order executed.";
}

//----Blockade Class----

//Deafult constructor for Blockade class
Blockade::Blockade() : Order() {
	this->targetTerritory = nullptr;
	cout << "Created a Blockade order." << endl;
}

//Parametized constructor for Blockade class
Blockade::Blockade(Player* player, Territory* target) : Order(player)
{
	this->targetTerritory = target;
}

//Destructor for Blockade class
Blockade::~Blockade()
{
	this->targetTerritory = nullptr;
}

//Copy constructor for Blockade class
Blockade::Blockade(const Blockade& toCopy) : Order(toCopy)
{
	this->targetTerritory = toCopy.targetTerritory;
}

//Assignment operator for Blockade class
Blockade& Blockade::operator=(const Blockade& rightSide)
{
	Order::operator=(rightSide);
	targetTerritory = rightSide.targetTerritory;
	return *this;
}

//Stream insertion operator for Blockade class
ostream& operator<<(ostream& out, const Blockade& toOutput)
{
	return toOutput.print(out);
}

ostream& Blockade::print(ostream& out) const
{
	out << "This is a Blockade order.";
	if (executed) {
		out << " This order was executed. The effect was Blockade effect.";
	}
	return out;
}

//Validate method for Blockade class
bool Blockade::validate()
{
	//TODO: Check that player has blockade card 
	return(targetTerritory->getOwnedBy() == this->getPlayer());
}

void Blockade::execute()
{
	if (!validate()) {
		cout << "Blockade order invalid. Will not execute" << endl;
		return;
	}
	//Set ownership to the neutral players and double armies
	targetTerritory->setOwnedBy(new Player(true), targetTerritory->getNumberOfArmies() * 2);
	targetTerritory->getOwnedBy()->removeTerritory(targetTerritory);
	executed = true;
	cout << "Blockade order executed. Territory" << targetTerritory->getName() << " has doubled its army units and is now owned by the neutral player" << endl;
    notify();
}
std::string Blockade::stringToLog() {
    return "ORDER::Execute(): Blockade order executed.";
}

//----Airlift Class----

//Deafult constructor for Airlift class
Airlift::Airlift() : Order() {
	sourceTerritory = nullptr;
	targetTerritory = nullptr;
	numOfArmyUnits = 0;
	cout << "Created an Airlift order." << endl;
}

//Parametized constructor for Airlift class
Airlift::Airlift(Player* player, Territory* source, Territory* target, unsigned int numOfArmyUnits) : Order(player)
{
	this->sourceTerritory = source;
	this->targetTerritory = target;
	this->numOfArmyUnits = numOfArmyUnits;
}

//Destructor for Airlift class
Airlift::~Airlift()
{
	this->sourceTerritory = nullptr;
	this->targetTerritory = nullptr;
}

//Copy constructor for Airlift class
Airlift::Airlift(const Airlift& toCopy) : Order(toCopy)
{
	this->sourceTerritory = toCopy.sourceTerritory;
	this->targetTerritory = toCopy.targetTerritory;
	this->numOfArmyUnits = toCopy.numOfArmyUnits;
}

//Assignment operator for Airlift class
Airlift& Airlift::operator=(const Airlift& rightSide)
{
	Order::operator=(rightSide);
	sourceTerritory = rightSide.sourceTerritory;
	targetTerritory = rightSide.targetTerritory;
	numOfArmyUnits = rightSide.numOfArmyUnits;
	return *this;
}

//Stream insertion operator for Airlift class
ostream& operator<<(ostream& out, const Airlift& toOutput)
{
	return toOutput.print(out);
}

ostream& Airlift::print(ostream& out) const
{
	out << "This is an Airlift order.";
	if (executed) {
		out << " This order was executed. The effect was Airlift effect.";
	}
	return out;
}

//Validate method for Airlift class
bool Airlift::validate()
{
	//TODO: Check if player has airlift card 
	return (sourceTerritory->getOwnedBy() == this->getPlayer() && targetTerritory->getOwnedBy() == this->getPlayer());
}

void Airlift::execute()
{
	if (!validate()) {
		cout << "Airlift order invalid. Will not execute" << endl;
		return;
	}
	sourceTerritory->removeArmies(numOfArmyUnits);
	targetTerritory->addArmies(numOfArmyUnits);
	executed = true;
	cout << "Airlift Order executed. " << numOfArmyUnits << " moved from " << sourceTerritory->getName() << " to " << targetTerritory->getName() << ". " << endl;
    notify();
}
std::string Airlift::stringToLog() {
    return "ORDER::Execute(): Airlift order executed.";
}

//----Negotiate Class----

//Deafult constructor for Negotiate class
Negotiate::Negotiate() : Order() {
	targetPlayer = nullptr;
	cout << "Created a Negotiate order." << endl;
}

//Parametized constructor for Negotiate class
Negotiate::Negotiate(Player* player, Player* target) : Order(player)
{
	this->targetPlayer = target;
}

//Destructor for Negotiate class
Negotiate::~Negotiate()
{
	this->targetPlayer = nullptr;
}

//Copy constructor for Negotiate class
Negotiate::Negotiate(const Negotiate& toCopy) : Order(toCopy)
{
	this->targetPlayer = toCopy.targetPlayer;
}

//Assignment operator for Negotiate class
Negotiate& Negotiate::operator=(const Negotiate& rightSide)
{
	Order::operator=(rightSide);
	targetPlayer = rightSide.targetPlayer;
	return *this;
}

//Stream insertion operator for Negotiate class
ostream& operator<<(ostream& out, const Negotiate& toOutput)
{
	return toOutput.print(out);
}

ostream& Negotiate::print(ostream& out) const
{
	out << "This is a Negotiate order.";
	if (executed) {
		out << " This order was executed. The effect was Negotiate effect.";
	}
	return out;
}

//Validate method for Negotiate class
bool Negotiate::validate()
{
	//TODO: Check that player has Negotiate card
	return(targetPlayer != this->getPlayer());
}

void Negotiate::execute()
{
	if (!validate()) {
		cout << "Negotiate order invalid. Will not execute" << endl;
		return;
	}
	//Add players to each others' negotiating lists
	this->getPlayer()->addNegotiator(targetPlayer);
	targetPlayer->addNegotiator(this->getPlayer());
	executed = true;
	cout << "Negotiate order executed." << endl;
    notify();
}
std::string Negotiate::stringToLog() {
    return "ORDER::Execute(): Negotiate order executed.";
}

//----OrdersList Class----

//Default constructor for OrdersList class
OrdersList::OrdersList()
{
}

OrdersList::OrdersList(std::list<Order*> order_list)
{
	this->order_list = order_list;
}

//Destructor for OrdersList class
OrdersList::~OrdersList()
{
	//Delete orders in list and orders_list
}

//Copy constructor for OrdersList class
OrdersList::OrdersList(const OrdersList& toCopy)
{
	this->order_list = std::list<Order*>(toCopy.order_list);
}

//Assignment operator for OrdersList class
OrdersList& OrdersList::operator=(const OrdersList& rightSide)
{
	order_list = rightSide.order_list;
	return *this;
}

//Stream insertion operator for OrdersList class
ostream& operator<<(ostream& out, const OrdersList& toOutput)
{
	std::list<Order*> order_list = toOutput.order_list;
	out << "List of Orders :" << std::endl;

	for (auto const& order : order_list) {
		std::cout << *order << std::endl;
	}

	return out;
}

//Method to remove order from list
void OrdersList::remove(int index)
{
	//Check if index out of bounds
	if (index < 0 || index >= order_list.size()) {
		cout << "Index out of bounds for remove" << endl;
		return;
	}

	std::list<Order*>::iterator itr = order_list.begin(); //Create an iterator for list
	advance(itr, index - 1); //Advance iterator to correct position (-1 because indices start at 0)
	order_list.erase(itr);//Delete element from list
	cout << "Removed order " << index << " from orders list." << endl;
}

//Method to move order in list
void OrdersList::move(int oldIndex, int newIndex)
{
	//Check if index out of bounds
	if (oldIndex < 0 || oldIndex >= order_list.size() || newIndex < 0 || newIndex > order_list.size()) {
		cout << "Index out of bounds to move." << endl;
		return;
	}
	std::list<Order*>::iterator old = order_list.begin();
	std::list<Order*>::iterator newPosition = order_list.begin();
	advance(old, oldIndex - 1);
	advance(newPosition, newIndex - 1);
	order_list.splice(newPosition, order_list, old);
	cout << "Moved order " << oldIndex << " to position " << newIndex << "." << endl;
}

//Method to add order to list
void OrdersList::addOrder(Order* o)
{
	order_list.push_back(o);
    notify();
}

std::string OrdersList::stringToLog() {
    return "ORDERLIST::addOrder(): order added."; //maybe to improve ??
}

std::list<Order*> OrdersList::getOrdersList()
{
	return this->order_list;
}