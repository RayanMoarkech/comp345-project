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
#include "../include/Map.h"
#include "../include/Cards.h"
#include "../include/Orders.h"
#include "../include/Player.h"


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
    _map = nullptr;
    _players = {};
}

GameEngine::GameEngine(const GameEngine &gameEngine) {
    _state = vector<State *>();
    for (auto const &state: gameEngine._state) {
        _state.push_back(new State(*state));
    }
    _currentStateIndex = gameEngine._currentStateIndex;
    _map = new Map(*gameEngine._map);
    _players = {};
    for (auto const &player: gameEngine._players) {
        _players.push_back(new Player(*player));
    }
}

GameEngine::~GameEngine() {
    for (State* state: _state)
    {
        delete state;
    }
    _state.clear();
    delete _map;
    for (Player* player: _players)
    {
        delete player;
    }
    _players.clear();
}

// TODO update
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

bool GameEngine::executeCurrentStateAction(string option)
{
    string currentStateName = this->_state[this->_currentStateIndex]->getName();
    // mapLoaded
    if (currentStateName == stateName[1])
    {
        this->_map = MapLoader::load(option);
        return true;
    }
    // mapValidated
    if (currentStateName == stateName[2])
    {
        this->_map->validate();
        return true;
    }
    // playersAdded
    if (currentStateName == stateName[3])
    {
        Player* player;
        if (option.empty())
        {
            player = new Player();
        }
        else
        {
            player = new Player(option, {}, new Hand(), new OrdersList());
        }
        cout << "Player " << player->getName() << " has been added." << endl;
        this->_players.push_back(player);
        return true;
    }
    return false;
}

void GameEngine::startupPhase()
{
    // The states that should be entered in order
//    vector<string> statesToEnter = {"loadmap", "validatemap", "addplayer", "gamestart"};
//
//    for (auto & stateToEnter : statesToEnter) {
//
//        //If the next index is -1, the game will terminate.
//        if (this->_currentStateIndex == -1) break;
//
//        cout << * this->_state[this->_currentStateIndex] << endl;
//
//        cout << "Auto-Command used: " << stateToEnter << endl;
//
//        // Set the current state index to the next state
//        // Based on the current state
//        for (auto const &transition: this->_state[this->_currentStateIndex]->getTransition())
//        {
//            if (stateToEnter == transition->getCommand())
//            {
//                this->_currentStateIndex = transition->getNextStateIndex();
//                this->executeCurrentStateAction();
//            }
//        }
//
//        cout << endl << endl;
//    }

    // The defined phases
    vector<string> phases = {"loadmap", "validatemap", "addplayer", "gamestart"};

    cout << "***************************************" << endl
        << "The following commands can be inserted:" << endl
        << "      1- loadmap <filename>            " << endl
        << "      2- validatemap                   " << endl
        << "      2- addplayer <playername>        " << endl
        << "      2- gamestart                     " << endl
        << "**********************************" << endl
        << endl;

    cout << "What is your command?" << endl
        << "......................." << endl
        << endl;

    //If the next index is -1, the game will terminate.
    while (this->_currentStateIndex != -1)
    {
        // Used as a flag to be true if the command is valid to allow going to the next state
        bool validCommand = false;

        cout << "Current state: " << this->_state[this->_currentStateIndex]->getName() << endl;
        cout << ">> ";

        // Get the userCommand
        string userCommand;
        getline(std::cin, userCommand);

        // Command
        string command = userCommand.substr(0, userCommand.find(' '));
        bool hasCommandOption = userCommand.find(' ') != string::npos;
        string commandOption = userCommand.substr(userCommand.find(' ') + 1);

        if (command == "gamestart")
        {
            vector<string> gamePhases = {};
        }

        // Check the user command against the valid commands at the current state
        // and set the current state index to the next state.
        for (auto const &transition: this->_state[this->_currentStateIndex]->getTransition())
        {
            if (command == transition->getCommand())
            {
                this->_currentStateIndex = transition->getNextStateIndex();
                validCommand = this->executeCurrentStateAction(hasCommandOption ? commandOption : "");
            }
        }

        //If the user command is invalid, print an error message
        if (!validCommand)
            cout << "\n-_- Your command: \"" + userCommand + "\" is invalid for the \"" +
                    this->_state[this->_currentStateIndex]->getName() + "\" state. -_-\n" << endl;
            //Print successful message if the transition was made.
        else
            cout << "\n" << (char) 1 << " Yay that was a valid transition " << (char) 1 << "\n" << endl;
    }
}

//Print a list of all states with their valid transitions
ostream &operator<<(ostream &strm, const GameEngine &gameEngine) {
    for (auto const &state: gameEngine._state) {
        strm << *state << endl;
    }
    return strm;
}
