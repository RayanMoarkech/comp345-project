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
        << "Test Human and Benevolent Strategies" << endl
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
    BenevolentPlayerStrategy* benevolentPlayer = new BenevolentPlayerStrategy(player2);

    player1->setPlayerStrategy(humanPlayer);
    player2->setPlayerStrategy(benevolentPlayer);

    gameEngine->_players = players;

    gameEngine->issueOrdersPhase();
    gameEngine->issueOrdersPhase();
}

void testBenevolentStrategy()
{
	cout << endl
			 << "------------------------------------------------------" << endl
			 << "Test Benevolent Strategy" << endl
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

	// Add Benevolent Player
	commandProcessor->saveCommand("addplayer BenevolentP1 benevolant");
	gameEngine->transition();

	commandProcessor->saveCommand("addplayer BenevolentP2 benevolant");
	gameEngine->transition();

	// Start Game phase
	commandProcessor->saveCommand("gamestart");
	gameEngine->transition();

	// Main Game Loop
	gameEngine->mainGameLoop();

	delete gameEngine;
}

void testCheaterStrategy()
{
	cout << endl
			 << "------------------------------------------------------" << endl
			 << "Test Cheater Strategy" << endl
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
	commandProcessor->saveCommand("addplayer CheaterP1 cheater");
	gameEngine->transition();

	commandProcessor->saveCommand("addplayer CheaterP2 cheater");
	gameEngine->transition();

	// Start Game phase
	commandProcessor->saveCommand("gamestart");
	gameEngine->transition();

	// Main Game Loop
	gameEngine->mainGameLoop();

	delete gameEngine;
}

void testAggressiveStrategy()
{
	cout << endl
			 << "------------------------------------------------------" << endl
			 << "Test Aggressive Strategy" << endl
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

	// Add Aggressive Player
	commandProcessor->saveCommand("addplayer AggressiveP1 aggressive");
	gameEngine->transition();

	commandProcessor->saveCommand("addplayer AggressiveP2 aggressive");
	gameEngine->transition();

	// Start Game phase
	commandProcessor->saveCommand("gamestart");
	gameEngine->transition();

	// Main Game Loop
	gameEngine->mainGameLoop();

	delete gameEngine;
}

void testNeutralStrategy()
{
	cout << endl
			 << "------------------------------------------------------" << endl
			 << "Test Neutral Strategy" << endl
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

	// Add Neutral Player
	commandProcessor->saveCommand("addplayer Neutral1 neutral");
	gameEngine->transition();

	commandProcessor->saveCommand("addplayer Neutral2 neutral");
	gameEngine->transition();

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

	// Add Benevolent Player
	commandProcessor->saveCommand("addplayer BenevolentP benevolant");
	gameEngine->transition();

	// Start Game phase
	commandProcessor->saveCommand("gamestart");
	gameEngine->transition();

	// Main Game Loop
	gameEngine->mainGameLoop();

	delete gameEngine;
}