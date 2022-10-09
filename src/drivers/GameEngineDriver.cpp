//
// COMP345_PROJECT_GAMEENGINEDRIVER_CPP GameEngineDriver.cpp
//

#include "../../include/GameEngine.h"
#include <iostream>
    using std::cout;
    using std::cin;
    using std::endl;

void testGameStates() {
	string stateName[]{ "start", "mapLoaded", "mapValidated", "playersAdded", "assignReinforcement", "issueOrders", "executeOrders", "win" };
	int currentStateIndex = 0;
	bool validCommand = false;
	string userCommand;
    GameEngine* gameEngine[8] = {
            new GameEngine(new State(stateName[0], { new Transition(stateName[0], "loadmap", 1)})),
            new GameEngine(new State(stateName[1], { new Transition(stateName[1], "loadmap", 1),
                                                     new Transition(stateName[1], "validatemap", 2) })),
            new GameEngine(new State(stateName[2], { new Transition(stateName[2], "addplayer", 3) })),
            new GameEngine(new State(stateName[3], { new Transition(stateName[3], "addplayer", 3),
                                                     new Transition(stateName[3], "assigncountries", 4)})),
            new GameEngine(new State(stateName[4], { new Transition(stateName[4], "issueorder", 5) })),
            new GameEngine(new State(stateName[5], { new Transition(stateName[5], "issueorder", 5),
                                                     new Transition(stateName[5], "endissueorders", 6)})),
            new GameEngine(new State(stateName[6], { new Transition(stateName[6], "execorder", 6),
                                                     new Transition(stateName[6], "endexecorders", 4),
                                                     new Transition(stateName[6], "win", 7)})),
            new GameEngine(new State(stateName[7], { new Transition(stateName[7], "play", 0),
                                                     new Transition(stateName[7], "end", -1)}))
	};


	cout << *gameEngine[currentStateIndex] << endl;

	cout << "What is your command?" << endl;
	cout << "........................" << endl;


	getline(cin, userCommand);
	while (cin) {

		for (auto const &transition : gameEngine[currentStateIndex]->getState()->getTransition() )
		{
			if (userCommand == transition->getCommand()) 
			{
				currentStateIndex = transition->getNextStateIndex();
				validCommand = true;
			}
		}

		if (currentStateIndex == -1) return;

		if (!validCommand)
			cout << "\n-_- Your command: \"" + userCommand + "\" is invalid for the \""+ gameEngine[currentStateIndex]->getState()->getName() +"\" state. -_-\n" << endl;
		else
			cout <<"\n" << (char)1 << " Yay that was a vild transition " << (char)1 << "\n" << endl;

		cout << *gameEngine[currentStateIndex] << endl;
		cout << "Please enter a new command!" << endl;
		cout << "........................" << endl;
		getline(cin, userCommand);
		validCommand = false;
	}
	cout << "Bye bye!" << endl;
}