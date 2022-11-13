//
// COMP345_PROJECT_GAMEENGINE_H GameEngine.h
//

#ifndef GameEngine_H
#define GameEngine_H

#include <string>
    using std::string;
#include <vector>
    using std::vector;
#include <ostream>
    using std::ostream;

#include "Cards.h"

class Map;
class Player;
class OrdersList;


// ---------------------------------------------
// ------------ Transition Section -------------
// Transaction defines the valid command to
// reach a state.
// ---------------------------------------------
class Transition {
public:
    Transition();
    ~Transition();
    Transition(string command, int nextStateIndex);
    Transition(const Transition&);
    Transition& operator=(const Transition&);


    string getCommand();
    int getNextStateIndex();

private:
    string _command;
    int _nextStateIndex;
    friend ostream& operator<<(ostream&, const Transition&);
};


// ---------------------------------------------
// --------------- State Section ---------------
// State holds the current state with a vector
// of valid transitions to go to next state.
// ---------------------------------------------

class State {
public:
    State();
    ~State();
    State(string name, vector<Transition*> transition);
    State(const State&);
    State& operator=(const State&);

    string getName();
    vector<Transition*> getTransition();

private:
    string _name;
    vector<Transition*> _transition;
    friend ostream& operator<<(ostream&, const State&);
};


// ---------------------------------------------
// ------------ GameEngine Section -------------
// GameEngine has a vector of all states to be
// used to keep track of the current state and
// navigate between them.
// ---------------------------------------------

class GameEngine {
public:
    GameEngine();
    ~GameEngine();
    GameEngine(const GameEngine &);
    GameEngine &operator=(const GameEngine &);

    vector<State*> getState();
    int getCurrentStateIndex();
    void setCurrentStateIndex(int currentStateIndex);

    void startupPhase();

    void reinforcementPhase(Map& map, Player& player);
    OrdersList* issueOrdersPhase(vector<Player*> players);

private:
    vector<State*> _state;
    int _currentStateIndex;

    Map* _map;
    vector<Player*> _players;
    Deck* deck;

    bool executeCurrentStateAction(int nextStateIndex, const string& option);
    bool nextState(string command, string commandOption);

    friend ostream &operator<<(ostream &, const GameEngine &);
};


#endif
