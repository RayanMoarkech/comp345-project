#include <iostream>
using std::cout;
using std::endl;
#include <typeinfo>

#include "../../include/PlayerStrategies.h"
#include "../../include/Player.h"
#include "../../include/GameEngine.h"

void testPlayerStrategies()
{
    cout << endl
        << "------------------------------------------------------" << endl
        << "Test Human and Benevolant Strategies" << endl
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

    player1->setGameEngine(gameEngine);
    player2->setGameEngine(gameEngine);

    //Populate owned territories 
    vector<Player*> players = { player1, player2 };

    player1->addOwnedTerritory(map->getTerritory("Cockpit01"));
    //player1->addOwnedTerritory(map->getTerritory("Cockpit02"));

    int counter = 0;
    for (Territory* t : map->getTerritories())
    {
        if (t->getContinent()->getName() == "hyperdrive")
        {
            player1->addOwnedTerritory(t);
        }
        if (t->getContinent()->getName() == "cockpit")
        {
            player2->addOwnedTerritory(t);
            t->setNumberOfArmies(counter);
        }
        counter++;
    }

    player1->setArmyUnits(20);
    player2->setArmyUnits(19);

    HumanPlayerStrategy* humanPlayer = new HumanPlayerStrategy(player1);
    BenevolentPlayerStrategy* benevolantPlayer = new BenevolentPlayerStrategy(player2);

    player1->setPlayerStrategy(humanPlayer);
    player2->setPlayerStrategy(benevolantPlayer);

    gameEngine->_players = players;

    gameEngine->issueOrdersPhase();
    gameEngine->issueOrdersPhase();
}