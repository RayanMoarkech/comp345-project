//
// Created by Lujain Khalaf on 2022-11-13.
//
#include "../../include/Player.h"
#include "../../include/Map.h"
#include "../../include/GameEngine.h"
#include <string>

#include "../../include/CommandProcessing.h"
#include <sstream>
using std::cout;
using std::endl;

void testLoggingObserver() {

        LogObserver* logger = new LogObserver();

#pragma region TestCommandProcessing

        cout << "------------------ Test: COMMAND PROCESSOR ------------------" << endl;

        cout << "Creating CommandProcessor" << endl;
        CommandProcessor* commandProcessor = new CommandProcessor();
//        commandProcessor->Attach(logger);

        cout << endl << "Obtaining command: ";
        Command* command = commandProcessor->getCommand();

        command->saveEffect("effect test");

//        delete commandProcessor;

#pragma endregion

#pragma region TestGameEngine

        cout << endl << "------------------ Test: GAME ENGINE ------------------" << endl;
        cout << endl << "Creating GameEngine" << endl;
        GameEngine *gameEngine = new GameEngine("");
        // attach logger to gameEngine here
        //

        delete gameEngine;

        // Will log the new state of the game engine (MAP_LOADED)
        cout << endl << "TRANSITION:  " << gameEngine->stringToLog() <<" to maploaded" << endl;
        gameEngine->getState();

        // Will log the new state of the game engine (MAP_VALIDATED)
        cout << endl << "TRANSITION " << gameEngine->stringToLog() << " to mapvalidated" << endl;
//        gameEngine->

        delete gameEngine;

#pragma endregion

#pragma region TestOrders

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

#pragma endregion

        delete logger;
        std::cin.get();
    }
