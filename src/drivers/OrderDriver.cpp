//
// COMP345_PROJECT_ORDERDRIVER_CPP OrderDriver.cpp
//

#include <iostream>
    using std::cout;

#include "../../include/Orders.h"
#include "../../include/Player.h"

void testOrdersList()
{
    cout << endl
        << "------------------------------------------------------" << endl
        << "Test Orders List" << endl
        << "------------------------------------------------------" << endl
        << endl;

	cout << "\nCreating an ordersList object:" << endl;
	OrdersList ordersList;

	cout << "\nCreating an order of each type:" << endl;
	Deploy deployOrder;
	Advance advanceOrder;
	Bomb bombOrder;
	Airlift airliftOrder;
	Blockade blockadeOrder;
	Negotiate negotiateOrder;

	cout << "\nPlacing orders in orders list object" << endl;
	ordersList.addOrder(&deployOrder);
	ordersList.addOrder(&advanceOrder);
	ordersList.addOrder(&bombOrder);
	ordersList.addOrder(&airliftOrder);
	ordersList.addOrder(&blockadeOrder);
	ordersList.addOrder(&negotiateOrder);
	cout << ordersList << endl;


	cout << "Testing remove method" << endl;
	ordersList.remove(2);
	cout << ordersList << endl;

	cout << "Testing move method" << endl;
	ordersList.move(2, 4);
	cout << ordersList << endl;

	cout << "Testing validate method." << endl;
	if (advanceOrder.validate()) { cout << "Validate was successful" << endl; }

	cout << "\nTesting execute method on validated order:" << endl;
	advanceOrder.execute();

	cout << "\nTesting execute method on unvalidated order:" << endl;
	deployOrder.execute();

	cout << "\nTesting stream insertion operator of executed order:" << endl;
	cout << advanceOrder << endl;

	cout << "\nTesting stream insertion operator of unexecuted order:" << endl;
	cout << deployOrder << endl;

}

void testOrderExecution() 
{
	cout << endl
		<< "------------------------------------------------------"
		<< endl
		<< "Test Order Execution" << endl
		<< "------------------------------------------------------" << endl
		<< endl;

	Player* player = new Player();
	Player* opponent = new Player();


}
