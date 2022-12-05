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
#include "../../include/CommandProcessing.h"


void testPlayerStrategies()
{
    cout << endl
        << "------------------------------------------------------" << endl
        << "Test Human and Benevolant Strategies" << endl
        << "------------------------------------------------------" << endl
        << endl;

		GameEngine* gameEngine = new GameEngine();

    Deck* deck = gameEngine->getDeck();
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

    gameEngine->issueOrdersPhase();
    gameEngine->issueOrdersPhase();
}

void testStrategies()
{
	cout << endl
			 << "------------------------------------------------------" << endl
			 << "Test Player Strategy" << endl
			 << "------------------------------------------------------" << endl
			 << endl;

	GameEngine* gameEngine = new GameEngine("");
	CommandProcessor* commandProcessor = gameEngine->getCommandProcessor();

	// Load the map
	commandProcessor->saveCommand("loadmap 001_I72_Ghtroc720.map");
	gameEngine->transition();

	// Validate the map
	commandProcessor->saveCommand("validatemap");
	gameEngine->transition();

	// Add Cheater Player
//	commandProcessor->saveCommand("addplayer CheaterP cheater");
//	gameEngine->transition();

	// Add Benevolant Player
	commandProcessor->saveCommand("addplayer BenevolantP benevolant");
	gameEngine->transition();


	commandProcessor->saveCommand("addplayer BenevolantP2 benevolant");
	gameEngine->transition();

	// Add Aggressive Player
//	commandProcessor->saveCommand("addplayer AggressiveP aggressive");
//	gameEngine->transition();

	// Add Neutral Player
//	commandProcessor->saveCommand("addplayer NeutralP neutral");
//	gameEngine->transition();

	// Start Game phase
	commandProcessor->saveCommand("gamestart");
	gameEngine->transition();

	// Main Game Loop
	gameEngine->mainGameLoop();

	delete gameEngine;
}

void testHumanStrategy()
{
	cout << endl
			 << "------------------------------------------------------" << endl
			 << "Test Human Strategy" << endl
			 << "------------------------------------------------------" << endl
			 << endl;

	GameEngine* gameEngine = new GameEngine("");
	CommandProcessor* commandProcessor = gameEngine->getCommandProcessor();

	// Load the map
	commandProcessor->saveCommand("loadmap 001_I72_Ghtroc720.map");
	gameEngine->transition();

	// Validate the map
	commandProcessor->saveCommand("validatemap");
	gameEngine->transition();

	// Add Cheater Player
	commandProcessor->saveCommand("addplayer HumanP human");
	gameEngine->transition();

	// Add Benevolant Player
	commandProcessor->saveCommand("addplayer BenevolantP benevolant");
	gameEngine->transition();

	// Start Game phase
	commandProcessor->saveCommand("gamestart");
	gameEngine->transition();

	// Main Game Loop
	gameEngine->mainGameLoop();

	delete gameEngine;
}