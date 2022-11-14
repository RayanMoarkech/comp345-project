//
// Created by AH2in1 on 2022-11-13.
//

#include "../../include/CommandProcessing.h"
#include "../../include/GameEngine.h"

#include <iostream>
using std::cin;
using std::cout;
using std::endl;

CommandProcessor *commandProcessorConsole = new CommandProcessor();
FileCommandProcessorAdapter *commandProcessorFile =
    new FileCommandProcessorAdapter("./commandFile.txt");

void TestCommandProcessor() {
  cout << endl
       << "------------------------------------------------------" << endl
       << "Test Command Processor - Console" << endl
       << "------------------------------------------------------" << endl
       << endl;

  // Used as a flag to be true if the command is valid to allow going to the
  // next state
  bool validCommand = false;
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

  while (gameEngine->getCurrentStateIndex() != -1) {
    // Check the user command against the valid commands at the current state
    //  and set the current state index to the next state.

    Command *commandObj = commandProcessorConsole->getCommand();
    string commandOption = "";
    validCommand = commandProcessorConsole->validate(
        commandObj, gameEngine->getCurrentStateIndex(),
        gameEngine->getNextStateIndex(), commandOption);

    // If the next index is -1, the game will terminate.
    if (gameEngine->getNextStateIndex() == -1)
      break;

    // If the user command is invalid, print an error message
    if (!validCommand) {
      string effect = "\n-_- Your command: \"" + commandObj->getUserCommand() +
                      "\" is invalid for the \"" +
                      gameEngine->getState()[gameEngine->getCurrentStateIndex()]
                          ->getName() +
                      "\" state. -_-\n";
      cout << effect << endl;
      commandProcessorConsole->getLastCommand()->saveEffect(effect);
    }

    // Print successful message if the transition was made.
    else {
      cout << "\n"
           << (char)1 << " Yay that was a valid transition " << (char)1 << "\n"
           << endl;
      gameEngine->setCurrentStateIndex(gameEngine->getNextStateIndex());
    }

    // keep ask the user to enter the new command if the command was invalid
    //  or to make another transition.
    cout << *gameEngine->getState()[gameEngine->getCurrentStateIndex()] << endl;
    cout << "Please enter a new command!" << endl;
    cout << "............................" << endl;
    cout << ">> ";
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
  //
  // Test File Command Processor Adapter.
  //

  cout << endl
       << "------------------------------------------------------" << endl
       << "Test Command Processor -file" << endl
       << "------------------------------------------------------" << endl
       << endl;

  // Used as a flag to be true if the command is valid to allow going to the
  // next state
  bool validCommandFile = false;
  GameEngine *gameEngineFile = new GameEngine("");
  cout << "\n\n"
          "**********************************\n"
          "*                                *\n"
          "*      Team DN08 - Warzone       *\n"
          "*                                *\n"
          "**********************************\n\n"
       << endl;

  cout << *gameEngineFile->getState()[gameEngineFile->getCurrentStateIndex()]
       << endl;
  cout << "What is your command?" << endl;
  cout << "......................." << endl;
  cout << ">> ";

  while (gameEngineFile->getCurrentStateIndex() != -1) {
    // Check the user command against the valid commands at the current state
    //  and set the current state index to the next state.

    Command *commandObj = commandProcessorFile->getCommand();
    string commandOption = "";
    validCommandFile = commandProcessorFile->validate(
        commandObj, gameEngineFile->getCurrentStateIndex(),
        gameEngineFile->getNextStateIndex(), commandOption);

    // If the next index is -1, the game will terminate.
    if (gameEngineFile->getNextStateIndex() == -1)
      break;

    // If the user command is invalid, print an error message
    if (!validCommandFile) {
      string effect =
          "\n-_- Your command: \"" + commandObj->getUserCommand() +
          "\" is invalid for the \"" +
          gameEngineFile->getState()[gameEngineFile->getCurrentStateIndex()]
              ->getName() +
          "\" state. -_-\n";
      cout << effect << endl;
      commandProcessorFile->getLastCommand()->saveEffect(effect);
    }

    // Print successful message if the transition was made.
    else {
      cout << "\n"
           << (char)1 << " Yay that was a valid transition " << (char)1 << "\n"
           << endl;
      gameEngineFile->setCurrentStateIndex(gameEngineFile->getNextStateIndex());
    }

    // keep ask the user to enter the new command if the command was invalid
    //  or to make another transition.
    cout << *gameEngineFile->getState()[gameEngineFile->getCurrentStateIndex()]
         << endl;
    cout << "Please enter a new command!" << endl;
    cout << "............................" << endl;
    cout << ">> ";
    validCommandFile = false;
  }

  cout << "\n"
          "**********************************\n"
          "*                                *\n"
          "*            Bye bye!            *\n"
          "*                                *\n"
          "**********************************\n"
       << endl;

  delete gameEngineFile;
}