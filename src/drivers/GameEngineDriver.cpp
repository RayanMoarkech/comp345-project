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
	State* states[8] = {
		new State(stateName[0], { new Transition(stateName[0], "loadmap", 1)}),
		new State(stateName[1], { new Transition(stateName[1], "loadmap", 1),
			new Transition(stateName[1], "validatemap", 2) }),
		new State(stateName[2], { new Transition(stateName[2], "addplayer", 3) }),
		new State(stateName[3], { new Transition(stateName[3], "addplayer", 3),
			new Transition(stateName[3], "assigncountries", 4)}),
		new State(stateName[4], { new Transition(stateName[4], "issueorder", 5) }),
		new State(stateName[5], { new Transition(stateName[5], "issueorder", 5),
			new Transition(stateName[5], "endissueorders", 6)}),
		new State(stateName[6], { new Transition(stateName[6], "execorder", 6),
			new Transition(stateName[6], "endexecorders", 4),
			new Transition(stateName[6], "win", 7)}),
		new State(stateName[7], { new Transition(stateName[7], "play", 0),
			new Transition(stateName[7], "end", -1)}),
	};


	cout << *states[currentStateIndex] << endl;

	cout << "What is your command?" << endl;
	cout << "........................" << endl;


	getline(cin, userCommand);
	while (cin) {

		for (auto const &transition : states[currentStateIndex]->getTransition() ) 
		{
			if (userCommand == transition->getCommand()) 
			{
				currentStateIndex = transition->getNextStateIndex();
				validCommand = true;
			}
		}

		if (currentStateIndex == -1) return;

		if (!validCommand)
			cout << "\n-_- Your command: \"" + userCommand + "\" is invalid for the \""+ states[currentStateIndex]->getName() +"\" state. -_-\n" << endl;
		else
			cout <<"\n" << (char)1 << " Yay that was a vild transition " << (char)1 << "\n" << endl;

		cout << *states[currentStateIndex] << endl;
		cout << "Please enter a new command!" << endl;
		cout << "........................" << endl;
		getline(cin, userCommand);
		validCommand = false;
	}
	cout << "Bye bye!" << endl;
}