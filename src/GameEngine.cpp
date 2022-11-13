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
#include <algorithm>
#include <random>

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

// Print the current state
ostream &operator<<(ostream &strm, const Transition &transition) {
    return strm << "- Use the \"" + transition._command + "\" command to go to the \"" +
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
    deck = nullptr;
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
    deck = new Deck(*gameEngine.deck);
}

GameEngine::~GameEngine() = default;

GameEngine &GameEngine::operator=(const GameEngine &gameEngine) {
    this->_state = gameEngine._state;
    this->_currentStateIndex = gameEngine._currentStateIndex;
    this->_map = gameEngine._map;
    this->_players = gameEngine._players;
    this->deck = gameEngine.deck;
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

bool GameEngine::nextState(string command, string commandOption)
{
    for (auto const &transition: this->_state[this->_currentStateIndex]->getTransition()) {
        if (command == transition->getCommand()) {
            const int nextStateIndex = transition->getNextStateIndex();
            const bool valid = executeCurrentStateAction(nextStateIndex, commandOption);
            if (valid)
                this->_currentStateIndex = nextStateIndex;
            else
                cout << "Invalid. Staying in the same state." << endl;
            return true;
        }
    }
    return false;
}

bool GameEngine::executeCurrentStateAction(int nextStateIndex, const string& option)
{
    string currentStateName = this->_state[this->_currentStateIndex]->getName();
    // Going to mapLoaded
    if (nextStateIndex == 1)
    {
        delete this->_map;
        this->_map = MapLoader::load(option);
        return true;
    }
    // Going to mapValidated
    if (nextStateIndex == 2)
    {
        const bool validMap = this->_map->validate();
        if (!validMap)
        {
            cout << "Map is not valid. Please try loading another one." << endl;
            return false;
        }
        cout << "Map is valid!" << endl;
        return true;
    }
    // Going to playersAdded
    if (nextStateIndex == 3)
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
    // Going to assignReinforcement: Distribute Territories and Reinforcements
    if (nextStateIndex == 4)
    {
        cout << "Fairly distributing the territories to the players" << endl;

        // Shuffling the territories
        vector<Territory*> shuffledTerritories = this->_map->getTerritories();
        auto rng = std::default_random_engine {};
        std::shuffle(std::begin(shuffledTerritories), std::end(shuffledTerritories), rng);
        int numberOfPlayer = this->_players.size();
        int count = 0;

        // Loop through players and add territories
        for (auto const &territory: shuffledTerritories)
        {
            Player* player = this->_players[count++];
            player->addOwnedTerritory(territory);
            territory->setOwnedBy(player, 0);
            // When reaching the end, repeat
            if (count == numberOfPlayer) { count = 0; }
        }
        return true;
    }
    // Going to issueOrders
    if (nextStateIndex == 5)
    {
        return true;
    }
    // Going to executeOrders
    if (nextStateIndex == 6)
    {
        return true;
    }
    // Going to win
    if (nextStateIndex == 7)
    {

    }
    return false;
}

void GameEngine::startupPhase()
{
    // The defined commands
    vector<string> commands = {"loadmap", "validatemap", "addplayer", "gamestart"};

    cout << "***************************************" << endl
        << "The following commands can be inserted:" << endl
        << "      1- loadmap <filename>            " << endl
        << "      2- validatemap                   " << endl
        << "      3- addplayer <playername>        " << endl
        << "      4- gamestart                     " << endl
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

        cout << *this->getState()[this->getCurrentStateIndex()];
        cout << ">> ";

        // Get the userCommand
        string userCommand;
        getline(std::cin, userCommand);

        // Command
        string command = userCommand.substr(0, userCommand.find(' '));
        bool hasCommandOption = userCommand.find(' ') != string::npos;
        string commandOption = userCommand.substr(userCommand.find(' ') + 1);

        // Check if command allowed
        for (const string allowedCommand: commands)
        {
            if (command == allowedCommand)
                validCommand = true;
        }

        if (validCommand && command == "gamestart" && this->_currentStateIndex == 3)
        {
            this->deck = new Deck();

            // 4-a fairly distribute all the territories to the players
            // 4-e switch the game to the play phase
            command = "assigncountries";
            this->nextState(command, "");

            // 4-b determine randomly the order of play of the players in the game
            cout << "Shuffling players..." << endl;
            auto rng = std::default_random_engine { std::random_device{}() };
            std::shuffle(std::begin(this->_players), std::end(this->_players), rng);
            cout << "Shuffled player order:" << endl;
            int count = 0;
            for (auto const player: this->_players)
            {
                cout << "\t" << std::to_string(count++) << ": " << player->getName() << endl;

                // 4-c give 50 initial army units to the players, which are placed in their respective reinforcement pool
                player->setArmyUnits(50);
                cout << "\t\tSetting " << player->getArmyUnits() << " army units." << endl;

                // 4-d let each player draw 2 initial cards from the deck using the deck’s draw() method
                Card *card1 = this->deck->draw();
                player->getPlayerHand()->addCard(card1);
                cout << "\t\tDraw Card 1: " << card1->getCardType() << endl;
                Card *card2 = this->deck->draw();
                player->getPlayerHand()->addCard(card2);
                cout << "\t\tDraw Card 2: " << card2->getCardType() << endl;
            }

            cout << endl
                << "Play phase..." << endl;

            //TODO: use in the play phase
//            command = "issueorder";
//            this->nextState(command, "");
//
//            command = "endissueorders";
//
//            this->nextState(command, "");
//            command = "execorder";
//            this->nextState(command, "");
//
//            command = "endexecorders";
//            this->nextState(command, "");

            break;
        }
        else if (validCommand)
        {
            validCommand = this->nextState(command, hasCommandOption ? commandOption : "");
        }

        // If the user command is invalid, print an error message
        if (!validCommand)
            cout << endl
                << "-_- Your command: \"" << userCommand << "\" is invalid for the \""
                << this->_state[this->_currentStateIndex]->getName() + "\" state. -_-"
                << endl;

        cout << endl;
    }
}

void GameEngine::reinforcementPhase(Map& map, vector<Player*> players)
{
    for (Player* player : players)
    {
        cout << player->getName() << " owns " << player->getOwnedTerritories().size() << " territories." << endl;

        int armyUnits = (player->getOwnedTerritories().size() / 3);

        if (armyUnits < 3)
        {
            player->setArmyUnits(3);
        }
        else
        {
            for (Continent* c : map.getContinents())
            {
                bool ownsContinent = false;
                for (Territory* t : map.getTerritories())
                {
                    if (c->getName() == t->getContinent()->getName())
                    {
                        for (Territory* ownedTerritory : player->getOwnedTerritories())
                        {
                            ownsContinent = ownedTerritory->getName() == t->getName();
                            if (ownsContinent)
                            {
                                break;
                            }
                        }
                    }
                }
                if (ownsContinent) {
                    cout << player->getName() << " owns continent " << c->getName() << " and gained " << c->getScore() << " army units." << endl;
                    armyUnits = +armyUnits + c->getScore();
                }
                else
                {
                    cout << player->getName() << " does not own continent " << c->getName() << "." << endl;
                }
            }
            player->setArmyUnits(armyUnits);
        }
        cout << player->getName() << " has " << player->getArmyUnits() << " army units this turn." << endl;
    }
}

//OrdersList* GameEngine::issueOrdersPhase(vector<Player*> players)
//{
//    OrdersList* allIssuedOrders = new OrdersList();
//
//    vector<string> orderTypes = { "toAttack", "toDefend", "Deploy", "AdvanceOrPlayCard" };
//    
//    //First all players need to choose the territories to Attack
//    for (Player* p : players)
//    {
//        p->issueOrder("toAttack");
//    }
//    //Then all players need to choose the territories to Defend
//    for (Player* p : players)
//    {
//        p->issueOrder("toDefend");
//    }
//    //Deploy
//    for (Player* p : players)
//    {
//        while (p->getArmyUnits() != 0)
//        {
//            allIssuedOrders->addOrder(p->issueOrder("Deploy"));
//        }
//    }
//    //Advance and Card Orders
//    int noMoreOrders = 0;
//    while (players.size() != noMoreOrders)
//    {
//        noMoreOrders = 0;
//
//        for (Player* p : players)
//        {
//            Order* advanceOrCardOrder = p->issueOrder("AdvanceOrPlayCard");
//            if (advanceOrCardOrder == NULL)
//            {
//                //all players must stop orders in the same turn
//                noMoreOrders++;
//            }
//            else {
//                allIssuedOrders->addOrder(advanceOrCardOrder);
//            }
//        }
//    }
//
//    return allIssuedOrders;
//}
//
//void GameEngine::executeOrdersPhase(OrdersList* allOrders)
//{
//    for (Order* o : allOrders->getOrdersList())
//    {
//        o->execute();
//    }
//}
//
//void GameEngine::mainGameLoop(Map& map, vector<Player*> players)
//{
//    cout << endl
//        << "------------------------------------------------------" << endl
//        << "Test Reinforcement Phase" << endl
//        << "------------------------------------------------------" << endl
//        << endl;
//    this->reinforcementPhase(map, players);
//    cout << endl
//        << "------------------------------------------------------" << endl
//        << "Issue Orders Phase" << endl
//        << "------------------------------------------------------" << endl
//        << endl;
//    OrdersList* allOrders = this->issueOrdersPhase(players);
//    //TO DO when merging with Part 4
//    //this->executeOrdersPhase(allOrders);
//
//}

//Print a list of all states with their valid transitions
ostream &operator<<(ostream &strm, const GameEngine &gameEngine) {
    for (auto const &state: gameEngine._state) {
        strm << *state << endl;
    }
    return strm;
}
