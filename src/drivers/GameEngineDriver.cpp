//
// COMP345_PROJECT_GAMEENGINEDRIVER_CPP GameEngineDriver.cpp
//


#include <iostream>
    using std::cout;
    using std::cin;
    using std::endl;

#include "../../include/GameEngine.h"
#include "../../include/Map.h"
#include "../../include/Player.h"


void testGameStates()
{
    cout << endl
        << "------------------------------------------------------" << endl
        << "Test Game States" << endl
        << "------------------------------------------------------" << endl
        << endl;

    //Used as a flag to be true if the command is valid to allow going to the next state
    bool validCommand = false;
    string userCommand;
    GameEngine *gameEngine = new GameEngine();
    cout << "\n\n"
            "**********************************\n"
            "*                                *\n"
            "*      Team DN08 - Warzone       *\n"
            "*                                *\n"
            "**********************************\n\n" << endl;


    cout << *gameEngine->getState()[gameEngine->getCurrentStateIndex()] << endl;
    cout << "What is your command?" << endl;
    cout << "......................." << endl;
    cout << ">> ";


    getline(cin, userCommand);
    while (cin) {
        //Check the user command against the valid commands at the current state
        // and set the current state index to the next state.
        for (auto const &transition: gameEngine->getState()[gameEngine->getCurrentStateIndex()]->getTransition()) {
            if (userCommand == transition->getCommand()) {
                gameEngine->setCurrentStateIndex(transition->getNextStateIndex());
                validCommand = true;
            }
        }

        //If the next index is -1, the game will terminate.
        if (gameEngine->getCurrentStateIndex() == -1) break;

        //If the user command is invalid, print an error message
        if (!validCommand)
            cout << "\n-_- Your command: \"" + userCommand + "\" is invalid for the \"" +
                    gameEngine->getState()[gameEngine->getCurrentStateIndex()]->getName() + "\" state. -_-\n" << endl;
        //Print successful message if the transition was made.
        else
            cout << "\n" << (char) 1 << " Yay that was a valid transition " << (char) 1 << "\n" << endl;

        //keep ask the user to enter the new command if the command was invalid
        // or to make another transition.
        cout << *gameEngine->getState()[gameEngine->getCurrentStateIndex()] << endl;
        cout << "Please enter a new command!" << endl;
        cout << "............................" << endl;
        cout << ">> ";
        getline(cin, userCommand);
        validCommand = false;
    }

    cout << "\n"
            "**********************************\n"
            "*                                *\n"
            "*            Bye bye!            *\n"
            "*                                *\n"
            "**********************************\n" << endl;

    delete gameEngine;
}

void testStartupPhase()
{
    cout << endl
         << "------------------------------------------------------" << endl
         << "Test Startup Phase" << endl
         << "------------------------------------------------------" << endl
         << endl;

    GameEngine *gameEngine = new GameEngine();
    gameEngine->startupPhase();
    delete gameEngine;
}

void testMainGameLoop()
{
    cout << endl
        << "------------------------------------------------------" << endl
        << "Test Main Game Loop" << endl
        << "------------------------------------------------------" << endl
        << endl;

    GameEngine* gameEngine = new GameEngine();
    Map* map = MapLoader::load("./001_I72_Ghtroc720.map");
    Player* player1 = new Player();
    Player* player2 = new Player();

    vector<Player*> players = { player1, player2 };

    player1->setName("Player 1");
    player2->setName("Player 2");

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
    gameEngine->reinforcementPhase(*map, players);
    
    while(true)
    for (Player* p : players)
    {
        p->issueOrder();
    }
    //gameEngine->mainGameLoop(*map, players);

}