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

#include "LoggingObserver.h"

class Map;
class Player;
class OrdersList;
class CommandProcessor;
class Command;
class Territory;
class Deck;
class Tournament;
class PlayerStrategy;


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
    Transition(const Transition &);
    Transition &operator=(const Transition &);

    string getCommand();
    int getNextStateIndex();

private:
    string _command;
    int _nextStateIndex;
    friend ostream &operator<<(ostream &, const Transition &);
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
    State(string name, vector<Transition *> transition);
    State(const State &);
    State &operator=(const State &);

    string getName();
    vector<Transition *> getTransition();

private:
    string _name;
    vector<Transition *> _transition;
    friend ostream &operator<<(ostream &, const State &);
};

// ---------------------------------------------
// ------------ GameEngine Section -------------
// GameEngine has a vector of all states to be
// used to keep track of the current state and
// navigate between them.
// ---------------------------------------------

class GameEngine: public ILoggable, public Subject {
public:
    GameEngine(string fileName="");
    ~GameEngine();
    GameEngine(const GameEngine &);
    GameEngine &operator=(const GameEngine &);

    Tournament* tournament;

    vector<State *> getState();
    int getCurrentStateIndex();
    int &getNextStateIndex();
    CommandProcessor* getCommandProcessor();
    void setCurrentStateIndex(int currentStateIndex);
    void setNextStateIndex(int nextStateIndex);
    Deck* getDeck();

    bool transition();
    bool startGame();
    void startupPhase();
    void reinforcementPhase();
    void issueOrdersPhase();
    void executeOrdersPhase();
    bool allPlayerCardsPlayed() const;
    int validateGameRound();
    int mainGameLoop(int maxLoop = 50); // Default maximum of loops to 50
    static vector<string> executeTournament(Tournament* t);

    vector<int> getOwnedTerritories(vector<int> ownedTerritory);

    std::string stringToLog();

    vector<Player*> _players;

private:
    vector<State *> _state;
    int _currentStateIndex;
    int _nextStateIndex;
    CommandProcessor *_commandProcessor;
    string _fileName;
    Map *_map;
    vector<PlayerStrategy *> _playerStrats;
    Deck *deck;
    OrdersList* ordersList;

    bool executeCurrentStateAction(int nextStateIndex, const string &option);

    friend ostream &operator<<(ostream &, const GameEngine &);
    friend class Tournament;
};

// ---------------------------------------------
// ------------ Tournament Section -------------
// Tournament holds the parameters necessary to
// execute tournament mode
// ---------------------------------------------
// ---------------------------------------------

class Tournament: public ILoggable, public Subject {
public:
    Tournament(vector<string>  mapArray, vector<string> playerStrategies, int numOfGames, int maxNumOfTurns);
    Tournament(const Tournament& tournament);
    Tournament& operator =(const Tournament& tournament);
    ~Tournament();

    friend ostream& operator << (ostream& out, const Tournament& t);
    std::string stringToLog();
private:
    friend class GameEngine;
    vector<string>  mapArray;
    vector<Map*> maps;
    vector<string> playerStrategies;
    int numOfGames;
    int maxNumOfTurns;
    vector<string> resultArray;
};

#endif
