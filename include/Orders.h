#pragma once

#include <iostream>
    using std::cout;
    using std::endl;
    using std::ostream;
#include <list>

#include "LoggingObserver.h"

class Player;
class Territory;

//Parent order class
class Order: public ILoggable, public Subject {
protected: // protected access specifies is used to ensure that members can be accessed by inherited classes
	bool executed; //Keeps track of whether order has been executed
	Player* player; //Player that the order belongs to
	virtual std::ostream& print(std::ostream& out) const; //Virtual function to delegate print to derived classes

public:
	Order(); //Default constructor for Order class
	Order(Player* player); //Parametrized constructor for Order class
	Order(const Order& toCopy); //Copy constructor

	/*
	* Destructor for Order Class
	* Declared as virtual so that both base class and derived class destructors called
	*/
	virtual ~Order();


	Order& operator=(const Order& o); //Assignment
	friend std::ostream& operator<<(std::ostream& out, const Order& toOutput); //Stream insertion operator
	Player* getPlayer(); // Get the player that an order belongs too

	/*
	* Pure virtual methods that will be overridden in derived classes
	*/
	virtual bool validate() = 0; // Method to check if order is valid
	virtual void execute() = 0; // Method to check if order is valid and then execute if it is
    virtual Territory * getTargetTerritory() = 0;
};

//Deploy Class Definition
class Deploy : public Order {
public:
	Deploy(); //Default Constructor
	Deploy(Player* player, Territory* territory, unsigned int numOfArmyUnits); //Parametized constructor
	~Deploy(); //Destructor
	Deploy(const Deploy& toCopy); //Copy constructor
	Deploy& operator=(const Deploy& rightSide); //Assignment operator
	friend std::ostream& operator<<(std::ostream& out, const Deploy& toOutput); //Stream insertion operator
	bool validate() override;
	void execute() override;
	std::string stringToLog() override; // Method for logging

private:
	Territory* targetTerritory;
	unsigned int numOfArmyUnits;
	virtual std::ostream& print(std::ostream& out) const override;
};

//Advance class definition
class Advance : public Order {
public:
	Advance(); //Default Constructor
	Advance(Player* player, Territory* source, Territory* target, unsigned int numOfArmyUnits); //Parametrized constructor
	~Advance(); //Destructor
	Advance(const Advance& toCopy); //Copy constructor
	Advance& operator=(const Advance& rightSide); //Assignment operator
	friend std::ostream& operator<<(std::ostream& out, const Advance& toOutput); //Stream insertion operator
	bool validate() override;
	void execute() override;
	std::string stringToLog() override; // Method for logging
    Territory *getTargetTerritory() const;

private:
	Territory* sourceTerritory;
	Territory* targetTerritory;
    unsigned int numOfArmyUnits;

	virtual std::ostream& print(std::ostream& out) const;
};

class Bomb : public Order {
public:
	Bomb(); //Default Constructor
	Bomb(Player* player, Territory* target); //Parametrized constructor
	~Bomb(); //Destructor
	Bomb(const Bomb& toCopy); //Copy constructor
	Bomb& operator=(const Bomb& rightSide); //Assignment operator
	friend std::ostream& operator<<(std::ostream& out, const Bomb& toOutput); //Stream insertion operator
	bool validate() override;
	void execute() override;
	virtual std::ostream& print(std::ostream& out) const;
	std::string stringToLog() override; // Method for logging
    Territory *getTargetTerritory() const;

private:
	Territory* targetTerritory;

};

class Blockade : public Order {
public:
	Blockade(); //Default Constructor
	Blockade(Player* player, Territory* target); //Parametrized constructor
	~Blockade(); //Destructor
	Blockade(const Blockade& toCopy); //Copy constructor
	Blockade& operator=(const Blockade& rightSide); //Assignment operator
	friend std::ostream& operator<<(std::ostream& out, const Blockade& toOutput); //Stream insertion operator
	bool validate() override;
	void execute() override;
	virtual std::ostream& print(std::ostream& out) const;
	std::string stringToLog() override; // Method for logging
private:
	Territory* targetTerritory;
};

class Airlift : public Order {
public:
	Airlift(); //Default Constructor
	Airlift(Player* player, Territory* source, Territory* target, unsigned int numOfArmyUnits); //Parametrized constructor
	~Airlift(); //Destructor
	Airlift(const Airlift& toCopy); //Copy constructor
	Airlift& operator=(const Airlift& rightSide); //Assignment operator
	friend std::ostream& operator<<(std::ostream& out, const Airlift& toOutput); //Stream insertion operator
	bool validate() override;
	void execute() override;
	std::string stringToLog() override; // Method for logging
private:
	unsigned int numOfArmyUnits;
	Territory* sourceTerritory;
	Territory* targetTerritory;
	virtual std::ostream& print(std::ostream& out) const;
};

class Negotiate : public Order {
public:
	Negotiate(); //Default Constructor
	Negotiate(Player* player, Player* target); //Parametrized constructor
	~Negotiate(); //Destructor
	Negotiate(const Negotiate& toCopy); //Copy constructor
	Negotiate& operator=(const Negotiate& rightSide); //Assignment operator
	friend std::ostream& operator<<(std::ostream& out, const Negotiate& toOutput); //Stream insertion operator
	bool validate() override;
	void execute() override;
	virtual std::ostream& print(std::ostream& out) const;
	std::string stringToLog() override; // Method for logging
private:
	Player* targetPlayer;
};

class OrdersList: public ILoggable, public Subject {
private:
	//Pointer to a list of order objects
	std::list<Order*> order_list;

public:
	//Default constructor for OrdersList
	OrdersList();

	//Parametrized constructor for OrdersList
	OrdersList(std::list<Order*> order_list);

	//OrdersList destructor
	~OrdersList();

	//Copy Constructor
	OrdersList(const OrdersList& toCopy);

	//Assignment Operator
	OrdersList& operator=(const OrdersList& rightSide);

	//Stream Insertion Operator
	friend std::ostream& operator<<(std::ostream& out, const OrdersList& toOutput);

	//Deletes the specified order from the list
	void remove(int index);

	//Moves the specified order to the specified position in list
	void move(int oldIndex, int newIndex);

	//Adds an order to the list
	void addOrder(Order* o);

	std::list<Order*> getOrdersList();

	// Method for logging
	std::string stringToLog();
};