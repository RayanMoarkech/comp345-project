#include <string>
#include <iostream>
	using std::cout;
	using std::endl;

#include "../../include/CommandProcessing.h"
#include "../../include/Player.h"
#include "../../include/Map.h"
#include "../../include/GameEngine.h"
#include "../../include/Orders.h"


void testLoggingObserver()
{

    LogObserver* logger = new LogObserver();

#pragma region TestCommandProcessing

    cout << "------------------ Test: COMMAND PROCESSOR ------------------" << endl;

    cout << "Creating CommandProcessor" << endl;
    CommandProcessor* commandProcessor = new CommandProcessor();
//        commandProcessor->Attach(logger);

    cout << endl << "Obtaining command: ";
    Command* command = commandProcessor->getCommand();

    command->saveEffect("effect test");

    delete commandProcessor;

#pragma endregion

#pragma region TestGameEngine

    cout << endl << "------------------ Test: GAME ENGINE ------------------" << endl;
    cout << endl << "Creating GameEngine" << endl;
    GameEngine *gameEngine = new GameEngine("");
    commandProcessor = gameEngine->getCommandProcessor();
    // attach logger to gameEngine here
    //

    // Will log the new state of the game engine (MAP_LOADED)
    cout << endl << "TRANSITION:  " << gameEngine->stringToLog() <<" to maploaded" << endl;
    commandProcessor->saveCommand("loadmap Cornwall.map");
    gameEngine->transition();

    // Will log the new state of the game engine (MAP_VALIDATED)
    cout << endl << "TRANSITION " << gameEngine->stringToLog() << " to mapvalidated" << endl;
    commandProcessor->saveCommand("validatemap");
    gameEngine->transition();

    delete gameEngine;

#pragma endregion

#pragma region TestOrders

    cout << endl << "------------------ Test: ORDERS ------------------" << endl;

    Map* map = MapLoader::load("Cornwall.map");

    cout << "\nCreating an ordersList object:" << endl;
    OrdersList ordersList;

    cout << "\nCreating an order of each type:" << endl;
    Deploy* deployOrder = new Deploy(new Player(), map->getTerritories()[0], 2);
    Advance* advanceOrder = new Advance(new Player(), map->getTerritories()[0], map->getTerritories()[1], 2);
    Bomb bombOrder;
    Airlift airliftOrder;
    Blockade blockadeOrder;
    Negotiate negotiateOrder;

    cout << "\nPlacing orders in orders list object" << endl;
    ordersList.addOrder(deployOrder);
    ordersList.addOrder(advanceOrder);
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
    if (advanceOrder->validate()) { cout << "Validate was successful" << endl; }

    cout << "\nTesting execute method on validated order:" << endl;
    advanceOrder->execute();

    cout << "\nTesting execute method on unvalidated order:" << endl;
    deployOrder->execute();

    cout << "\nTesting stream insertion operator of executed order:" << endl;
    cout << advanceOrder << endl;

    cout << "\nTesting stream insertion operator of unexecuted order:" << endl;
    cout << deployOrder << endl;

    delete map;
    delete advanceOrder;
    delete deployOrder;

#pragma endregion

    delete logger;
    std::cin.get();
}
