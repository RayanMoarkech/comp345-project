//
// COMP345_PROJECT_GAMEENGINE_CPP GameEngine.cpp
//

#include <string>
    using std::string;
#include <vector>
    using std::vector;
#include <iostream>
    using std::cout;
    using std::endl;

#include "../include/GameEngine.h"


// Holds the state names to be used across class
static const string stateName[]{ "start", "mapLoaded", "mapValidated", "playersAdded", "assignReinforcement", "issueOrders", "executeOrders", "win" };

// ---------------------------------------------
// ------------ Transition Section -------------
// ---------------------------------------------

Transition::Transition() {}

Transition::~Transition() {}

Transition::Transition( string command, int nextStateIndex) {
    _command = command;
    _nextStateIndex = nextStateIndex;

}

Transition::Transition(const Transition &transition) {
    _command = transition._command;
    _nextStateIndex = transition._nextStateIndex;
}

Transition &Transition::operator=(const Transition &transition) {
    _command = transition._command;
    _nextStateIndex = transition._nextStateIndex;
    return *this;
}

int Transition::getNextStateIndex() {
    return _nextStateIndex;
}

string Transition::getCommand() {
	return _command;
}

//Print the current satate
ostream &operator<<(ostream &strm, const Transition &transition) {
    return strm << "-Use the \"" + transition._command + "\" command to go to the \"" +
            stateName[transition._nextStateIndex] + "\" state.";
}


// ---------------------------------------------
// --------------- State Section ---------------
// ---------------------------------------------

State::State() {
    _name = nullptr;
    vector<Transition*> _transition ;
}

State::~State() {
    for (Transition* transition: _transition)
    {
        delete transition;
        transition = nullptr;
    }
    _transition.clear();
}

State::State(string name, vector<Transition *> transition) {
    _name = std::move(name);
    _transition = std::move(transition);
}

State::State(const State &state) {
    _name = state._name;
    _transition = vector<Transition *>();
    for (auto const &transition: state._transition) {
        _transition.push_back(new Transition(*transition));
    }
}

State &State::operator=(const State &state) {
    _name = state._name;
    _transition = state._transition;
    return *this;
}

string State::getName() {
    return _name;
}

vector<Transition *> State::getTransition() {
    return _transition;
}

//Print a list of valid transitions, using the transition print method,
//at the current state
ostream &operator<<(ostream &strm, const State &state) {
    strm << "The current state is \"" + state._name + "\", and has these/this valid transition/s:" << endl;
    for (auto &transition: state._transition)
        strm << *transition << endl;
    return strm;
}


// ---------------------------------------------
// ------------ GameEngine Section -------------
// ---------------------------------------------

//Create the gameEngine object with all the states with their valid transitions
GameEngine::GameEngine() {
    _state = {
            new State(stateName[0], { new Transition("loadmap", 1)}),
            new State(stateName[1], { new Transition("loadmap", 1),
                                      new Transition("validatemap", 2) }),
            new State(stateName[2], { new Transition("addplayer", 3) }),
            new State(stateName[3], { new Transition("addplayer", 3),
                                      new Transition("assigncountries", 4)}),
            new State(stateName[4], { new Transition("issueorder", 5) }),
            new State(stateName[5], { new Transition("issueorder", 5),
                                      new Transition("endissueorders", 6)}),
            new State(stateName[6], { new Transition("execorder", 6),
                                      new Transition("endexecorders", 4),
                                      new Transition("win", 7)}),
            new State(stateName[7], { new Transition("play", 0),
                                      new Transition("end", -1)}),
    };
    _currentStateIndex = 0;
}

GameEngine::GameEngine(const GameEngine &gameEngine) {
    _state = vector<State *>();
    for (auto const &state: gameEngine._state) {
        _state.push_back(new State(*state));
    }
    _currentStateIndex = gameEngine._currentStateIndex;
}
GameEngine::~GameEngine() {
    for (State* state: _state)
    {
        delete state;
        state = nullptr;
    }
    _state.clear();
}
GameEngine &GameEngine::operator=(const GameEngine &gameEngine) {
    _state = gameEngine._state;
    return *this;
}


vector<State*> GameEngine::getState() {
    return _state;
}

int GameEngine::getCurrentStateIndex() {
    return _currentStateIndex;
}

void GameEngine::setCurrentStateIndex(int currentStateIndex) {
    _currentStateIndex = currentStateIndex;
}

void GameEngine::executeCurrentStateAction()
{

}

void GameEngine::startupPhase()
{
    //Used as a flag to be true if the command is valid to allow going to the next state
    bool validCommand = false;
    string userCommand;

    cout << endl
        << "**********************************" << endl
        << "*                                *" << endl
        << "*      Team DN08 - Warzone       *" << endl
        << "*                                *" << endl
        << "**********************************" << endl << endl << endl;


    cout << this->_state[this->_currentStateIndex] << endl
        << "What is your command?" << endl
        << "......................." << endl
        << ">> ";


    getline(std::cin, userCommand);
    while (std::cin) {
        // Check the user command against the valid commands at the current state
        // and set the current state index to the next state.
        for (auto const &transition: this->_state[this->_currentStateIndex]->getTransition()) {
            if (userCommand == transition->getCommand()) {
                this->_currentStateIndex = transition->getNextStateIndex();
                this->executeCurrentStateAction();
                validCommand = true;
            }
        }

        //If the next index is -1, the game will terminate.
        if (this->_currentStateIndex == -1) break;

        //If the user command is invalid, print an error message
        if (!validCommand)
            cout << "\n-_- Your command: \"" + userCommand + "\" is invalid for the \"" +
                    this->_state[this->_currentStateIndex]->getName() + "\" state. -_-\n" << endl;
            //Print successful message if the transition was made.
        else
            cout << "\n" << (char) 1 << " Yay that was a valid transition " << (char) 1 << "\n" << endl;

        //keep ask the user to enter the new command if the command was invalid
        // or to make another transition.
        cout << this->_state[this->_currentStateIndex] << endl;
        cout << "Please enter a new command!" << endl;
        cout << "............................" << endl;
        cout << ">> ";
        getline(std::cin, userCommand);
        validCommand = false;
    }

    cout << endl
        << "**********************************" << endl
        << "*                                *" << endl
        << "*            Bye bye!            *" << endl
        << "*                                *" << endl
        << "**********************************" << endl
        << endl;
}

//Print a list of all states with their valid transitions
ostream &operator<<(ostream &strm, const GameEngine &gameEngine) {
    for (auto const &state: gameEngine._state) {
        strm << *state << endl;
    }
    return strm;
}
