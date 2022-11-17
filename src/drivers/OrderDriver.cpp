//
// COMP345_PROJECT_ORDERDRIVER_CPP OrderDriver.cpp
//

#include <iostream>
    using std::cout;

#include "../../include/Orders.h"
#include "../../include/Player.h"
#include "../../include/GameEngine.h"

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
		<< "------------------------------------------------------" << endl
		<< "Test Order Execution" << endl
		<< "------------------------------------------------------" << endl
		<< endl;

    GameEngine* gameEngine = new GameEngine("");
    Map* map = MapLoader::load("./001_I72_Ghtroc720.map");

    Hand* playerOneHand = new Hand();
    Hand* playerTwoHand = new Hand();

    Card* bombCard = new Card(BOMB);
    Card* blockadeCard = new Card(BLOCKADE);
    Card* airliftCard = new Card(AIRLIFT);

    playerOneHand->addCard(bombCard);
    playerOneHand->addCard(blockadeCard);
    playerTwoHand->addCard(airliftCard);

    string playerOneName = "Player 1";
    vector<Territory*> playerOneTerritories;

    string playerTwoName = "Player 2";
    vector<Territory*> playerTwoTerritories;

    Player* player1 = new Player(playerOneName, playerOneTerritories, playerOneHand, new OrdersList());
    Player* player2 = new Player(playerTwoName, playerTwoTerritories, playerTwoHand, new OrdersList());;

    //Populate owned territories 
    vector<Player*> players = { player1, player2 };

    player1->addOwnedTerritory(map->getTerritory("Cockpit01"));
    player1->addOwnedTerritory(map->getTerritory("Cockpit02"));

    for (Territory* t : map->getTerritories())
    {
        if (t->getContinent()->getName() == "hyperdrive")
        {
            player1->addOwnedTerritory(t);
        }
        if (t->getContinent()->getName() == "cockpit")
        {
            player2->addOwnedTerritory(t);
        }
    }
	
	vector<Territory*> territories = map->getTerritories();
	Territory* sourceTerritory = territories[0];
	Territory* targetTerritory = territories[1];
	Territory* territory3 = territories[2];
	Territory* territory4 = territories[24];

	targetTerritory->setOwnedBy(player1, 15);
	sourceTerritory->setOwnedBy(player1, 20);
	territory3->setOwnedBy(player2, 3);
	territory4->setOwnedBy(player2, 12);

	Deploy* deployOrder = new Deploy(player1, targetTerritory, 10);
	deployOrder->execute();

	Advance* advanceOrder = new Advance(player1, sourceTerritory, targetTerritory, 5);
	advanceOrder->execute();

	Bomb* bombOrder = new Bomb(player1, sourceTerritory);
	bombOrder->execute();
}
