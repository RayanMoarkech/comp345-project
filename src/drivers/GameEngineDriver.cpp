//
// COMP345_PROJECT_GAMEENGINEDRIVER_CPP GameEngineDriver.cpp
//


#include <iostream>
    using std::cout;
    using std::cin;
    using std::endl;
#include <string>
    using std::string;

#include "../../include/GameEngine.h"
#include "../../include/Map.h"
#include "../../include/Player.h"
#include "../../include/Cards.h"



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

    Deck* gameDeck = new Deck();
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
    gameEngine->reinforcementPhase(*map, players);

    player1->toAttack();
    cout << player1->getAttackList().size() << endl;
    player2->toAttack();
    cout << player2->getAttackList().size() << endl;

    //while(player1->getAttackList().size() != 1 || player2->getAttackList().size() != 1)
    while (player1->getPlayerHand()->cards.size() != 0 || player2->getPlayerHand()->cards.size() != 0)
    for (Player* p : players)
    {
        
        if (p->getAttackList().size() == 1 && p->getPlayerHand()->cards.size() != 0)
        {
            p->getPlayerHand()->cards.at(0)->play(p, gameDeck);
        }
        else
        {
            p->issueOrder();
        }

    }
    //gameEngine->mainGameLoop(*map, players);

}