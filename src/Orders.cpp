//
// COMP345_PROJECT_ORDERS_CPP Orders.cpp
//

#include "../include/Orders.h"
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

//Deep copy constructor for Order parent class
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

//Validate method for Order base class
bool Order::validate()
{
	return false;// A1
}

//Execute method for Order base class
void Order::execute()
{
	if (!validate()) {
		cout << "Order invalid. Will not execute." << endl;
		return;
	}
	executed = true;
	//TODO: Insert execution behaviour
	cout << "Order executed." << endl;
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

//Deep copy constructor for Deploy class
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
	// return (targetTerritory.getPlayer() == player); //Check that the address of targetTerritory's player is the same address of this player
	return false; //Temporary for A1 testing
}

//Method to validate and execute deploy order
void Deploy::execute()
{
	if (!validate()) {
		cout << "Deploy order invalid. Will not execute." << endl;
		return;
	}
	executed = true;
	//TODO: Insert execution behaviour
	cout << "Deploy order executed." << endl;
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

//Deep copy constructor for Advance class
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
	//Check if source territory belongs to player and that territories are adjacent
	//TODO isAdjacent method in territory
	//return (sourceTerritory.getPlayer() == player && sourceTerriotry.isAdjacent(targetTerritory));
	return true; //Temporary for A1 testing

}

//Execute method for Advance class
void Advance::execute()
{
	if (!validate()) {
		cout << "Advance order invalid. Will not execute" << endl;
		return;
	}
	executed = true;
	//TODO: Insert execution behaviour
	cout << "Advance order executed." << endl;
}


//----Bomb Class----

//Deafult constructor for Bomb class
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

//Deep copy constructor for Bomb class
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
	//Check that player has bomb card,target Territory does not belong to player and is adjacent to one of the player's current territories
	//TODO Player checkHand() method, Territory getPlayer method and adjacentToPlayer method
	//return (player.checkHand("Bomb") && targetTerritory.getPlayer() != player && targetTerritory.adjacentToPlayer());
	return true; //Temporary for A1 testing
}

void Bomb::execute()
{
	if (!validate()) {
		cout << "Bomb order invalid. Will not execute" << endl;
		return;
	}
	executed = true;
	//TODO: Insert execution behaviour
	cout << "Bomb order executed." << endl;
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

//Deep copy constructor for Blockade class
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
	//Check that player has blockade card and territory belongs to player
	//TODO Player checkHand(), Territory getPlayer method and adjacentToPlayer method
	//return (player.checkHand("Blockade") && targetTerritory.getPlayer() == player);
	return true; //Temporary for A1 testing
}

void Blockade::execute()
{
	if (!validate()) {
		cout << "Blockade order invalid. Will not execute" << endl;
		return;
	}
	executed = true;
	//TODO: Insert execution behaviour
	cout << "Blockade order executed." << endl;
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

//Deep copy constructor for Airlift class
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
	//Check if player has airlift card and source territory belongs to player
	//TODO Player checkHand() and Territory getPlayer()
	//return ((player.checkHand("Airlift") && sourceTerritory.getPlayer() == player));
	return true; //Temporary for A1 testing
}

void Airlift::execute()
{
	if (!validate()) {
		cout << "Airlift order invalid. Will not execute" << endl;
		return;
	}
	executed = true;
	//TODO: Insert execution behaviour
	cout << "Airlift order executed." << endl;
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

//Deep copy constructor for Negotiate class
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
	//Check that player has Negotiate card and current and target player are not the same
	//TODO Player checkHand() method
	//return ((player.checkHand("Diplomacy") && targetPlayer != player));
	return true; //Temporary for A1 testing
}

void Negotiate::execute()
{
	if (!validate()) {
		cout << "Negotiate order invalid. Will not execute" << endl;
		return;
	}
	executed = true;
	//TODO: Insert execution behaviour
	cout << "Negotiate order executed." << endl;
}

//----OrdersList Class----

//Default constructor for OrdersList class
OrdersList::OrdersList()
{
	cout << "Created an ordersList object." << endl;
}

OrdersList::OrdersList(std::list<Order*> order_list)
{
	this->order_list = order_list;
}

//Destructor for OrdersList class
OrdersList::~OrdersList()
{
	//Delete orders in list and orders_list
	/*
	* for (auto const& order : order_list) {
		delete order;
	}
	*/
}

//Deep copy constructor for OrdersList class
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
	advance(itr, index - 1); //Advancee iterator to correct position (-1 because indices start at 0)
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
	//TODO: Change splice
	order_list.splice(newPosition, order_list, old);
	cout << "Moved order " << oldIndex << " to position " << newIndex << "." << endl;
}

//Method to add order to list
void OrdersList::addOrder(Order* o)
{
	order_list.push_back(o);
}

std::list<Order*> OrdersList::getOrdersList()
{
	return this->order_list;
}