#include <iostream>
	using std::cout;
	using std::endl;
#include <typeinfo>

#include "../../include/PlayerStrategies.h"
#include "../../include/Player.h"
#include "../../include/GameEngine.h"
#include "../../include/Map.h"
#include "../../include/Orders.h"
#include "../../include/Cards.h"


void testPlayerStrategies()
{
    cout << endl
        << "------------------------------------------------------" << endl
        << "Test Human and Benevolant Strategies" << endl
        << "------------------------------------------------------" << endl
        << endl;

    Deck* deck = new Deck();
    Map* map = MapLoader::load("./001_I72_Ghtroc720.map");

    Hand* playerOneHand = new Hand();
    Hand* playerTwoHand = new Hand();

    Card* bombCard = new Card(BOMB);
		bombCard->setDeck(deck);
    Card* blockadeCard = new Card(BLOCKADE);
		blockadeCard->setDeck(deck);
    Card* airliftCard = new Card(AIRLIFT);
		airliftCard->setDeck(deck);

    playerOneHand->addCard(bombCard);
    playerOneHand->addCard(blockadeCard);
    playerTwoHand->addCard(airliftCard);

    string playerOneName = "Player 1";
    vector<Territory*> playerOneTerritories;

    string playerTwoName = "Player 2";
    vector<Territory*> playerTwoTerritories;

    Player* player1 = new Player(playerOneName, playerOneTerritories, playerOneHand, new OrdersList());
    Player* player2 = new Player(playerTwoName, playerTwoTerritories, playerTwoHand, new OrdersList());

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

    //for (int i=0; i < 40; i++)
    //{
    //    player1->issueOrder();
    //    player2->issueOrder();
    //}

    /*bool allPlayersDone = false;
    OrdersList* allOrders = new OrdersList();

    while (!allPlayersDone)
    {
        allOrders->addOrder(player1->issueOrder());
        allOrders->addOrder(player2->issueOrder());

        
        for (int i = 0; i < players.size(); i++)
        {
            if (players.at(i)->getArmyUnits() == 0 && !allOrders->getOrdersList().back())
            {
                allPlayersDone = true;
                allOrders->getOrdersList().pop_back();
            }
            else {
                allPlayersDone = false;
                break;
            }
        }
    }
    for (Order* o: allOrders->getOrdersList())
    {
        if (o != nullptr)
        {
            cout << o->getPlayer()->getName() << endl;
        }
    }*/

    gameEngine->issueOrdersPhase();
}