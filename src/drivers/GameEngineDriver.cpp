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
#include "../../include/CommandProcessing.h"


void testGameStates() {
  cout << endl
       << "------------------------------------------------------" << endl
       << "Test Game States" << endl
       << "------------------------------------------------------" << endl
       << endl;

  // Used as a flag to be true if the command is valid to allow going to the
  // next state
  bool validCommand = false;
  string userCommand;
  GameEngine *gameEngine = new GameEngine("");
  cout << "\n\n"
          "**********************************\n"
          "*                                *\n"
          "*      Team DN08 - Warzone       *\n"
          "*                                *\n"
          "**********************************\n\n"
       << endl;

  cout << *gameEngine->getState()[gameEngine->getCurrentStateIndex()] << endl;
  cout << "What is your command?" << endl;
  cout << "......................." << endl;
  cout << ">> ";

  getline(cin, userCommand);
  while (cin) {
    // Check the user command against the valid commands at the current state
    //  and set the current state index to the next state.
    for (auto const &transition :
         gameEngine->getState()[gameEngine->getCurrentStateIndex()]
             ->getTransition()) {
      if (userCommand == transition->getCommand()) {
        gameEngine->setCurrentStateIndex(transition->getNextStateIndex());
        validCommand = true;
      }
    }

    // If the next index is -1, the game will terminate.
    if (gameEngine->getCurrentStateIndex() == -1)
      break;

    // If the user command is invalid, print an error message
    if (!validCommand)
      cout << "\n-_- Your command: \"" + userCommand +
                  "\" is invalid for the \"" +
                  gameEngine->getState()[gameEngine->getCurrentStateIndex()]
                      ->getName() +
                  "\" state. -_-\n"
           << endl;
    // Print successful message if the transition was made.
    else
      cout << "\n"
           << (char)1 << " Yay that was a valid transition " << (char)1 << "\n"
           << endl;

    // keep ask the user to enter the new command if the command was invalid
    //  or to make another transition.
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
          "**********************************\n"
       << endl;

  delete gameEngine;
}

void testStartupPhase(string fileName) {
    cout << endl
        << "------------------------------------------------------" << endl
        << "Test Startup Phase" << endl
        << "------------------------------------------------------" << endl
        << endl;

    GameEngine *gameEngine = new GameEngine("");
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

    GameEngine* gameEngine = new GameEngine("");
    CommandProcessor* commandProcessor = gameEngine->getCommandProcessor();

    // Load the map
    commandProcessor->saveCommand("loadmap 001_I72_Ghtroc720.map");
    gameEngine->transition();

    // Validate the map
    commandProcessor->saveCommand("validatemap");
    gameEngine->transition();

    // Add players
    commandProcessor->saveCommand("addplayer Player1");
    gameEngine->transition();

    commandProcessor->saveCommand("addplayer Player2");
    gameEngine->transition();

    // Start Game phase
    commandProcessor->saveCommand("gamestart");
    gameEngine->transition();

    // Main Game Loop
    gameEngine->mainGameLoop();

    delete gameEngine;
}