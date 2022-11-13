//
// COMP345_PROJECT_GAMEENGINEDRIVER_CPP CommandProcessingDriver.cpp
//


#include <iostream>
    using std::cout;
    using std::cin;
    using std::endl;

#include "../../include/CommandProcessing.h"

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
    CommandProcessing *commandProcessing = new CommandProcessing();
    cout << "\n\n"
            "**********************************\n"
            "*                                *\n"
            "*      Team DN08 - Warzone       *\n"
            "*                                *\n"
            "**********************************\n\n" << endl;


    cout << *commandProcessing->getState()[commandProcessing->getCurrentStateIndex()] << endl;
    cout << "What is your command?" << endl;
    cout << "......................." << endl;
    cout << ">> ";


    getline(cin, userCommand);
    while (cin) {
        //Check the user command against the valid commands at the current state
        // and set the current state index to the next state.
        for (auto const &transition: commandProcessing->getState()[commandProcessing->getCurrentStateIndex()]->getTransition()) {
            if (userCommand == transition->getCommand()) {
                commandProcessing->setCurrentStateIndex(transition->getNextStateIndex());
                validCommand = true;
            }
        }

        //If the next index is -1, the game will terminate.
        if (commandProcessing->getCurrentStateIndex() == -1) break;

        //If the user command is invalid, print an error message
        if (!validCommand)
            cout << "\n-_- Your command: \"" + userCommand + "\" is invalid for the \"" +
                    commandProcessing->getState()[commandProcessing->getCurrentStateIndex()]->getName() + "\" state. -_-\n" << endl;
        //Print successful message if the transition was made.
        else
            cout << "\n" << (char) 1 << " Yay that was a valid transition " << (char) 1 << "\n" << endl;

        //keep ask the user to enter the new command if the command was invalid
        // or to make another transition.
        cout << *commandProcessing->getState()[commandProcessing->getCurrentStateIndex()] << endl;
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

    delete commandProcessing;
}