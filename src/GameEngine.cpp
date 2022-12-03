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

#include "../include/Cards.h"
#include "../include/CommandProcessing.h"
#include "../include/GameEngine.h"
#include "../include/Map.h"
#include "../include/Orders.h"
#include "../include/Player.h"

// Holds the state names to be used across class
static const string stateName[]{"start",
                                "mapLoaded",
                                "mapValidated",
                                "playersAdded",
                                "assignReinforcement",
                                "issueOrders",
                                "executeOrders",
                                "win"};

// ---------------------------------------------
// ------------ Transition Section -------------
// ---------------------------------------------

Transition::Transition() {}

Transition::~Transition() {}

Transition::Transition(string command, int nextStateIndex) {
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

int Transition::getNextStateIndex() { return _nextStateIndex; }

string Transition::getCommand() { return _command; }

// Print the current state
ostream &operator<<(ostream &strm, const Transition &transition) {
  return strm << "- Use the \"" + transition._command +
                     "\" command to go to the \"" +
                     stateName[transition._nextStateIndex] + "\" state.";
}

// ---------------------------------------------
// --------------- State Section ---------------
// ---------------------------------------------

State::State() {
  _name = nullptr;
  vector<Transition *> _transition;
}

State::~State() {
  for (Transition *transition : _transition) {
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
  for (auto const &transition : state._transition) {
    _transition.push_back(new Transition(*transition));
  }
}

State &State::operator=(const State &state) {
  _name = state._name;
  _transition = state._transition;
  return *this;
}

string State::getName() { return _name; }

vector<Transition *> State::getTransition() { return _transition; }

// Print a list of valid transitions, using the transition print method,
// at the current state
ostream &operator<<(ostream &strm, const State &state) {
  strm << "The current state is \"" + state._name +
              "\", and has these/this valid transition/s:"
       << endl;
  for (auto &transition : state._transition)
    strm << *transition << endl;
  return strm;
}

// ---------------------------------------------
// ------------ GameEngine Section -------------
// ---------------------------------------------

// Create the gameEngine object with all the states with their valid transitions
GameEngine::GameEngine(string fileName) {
  _state = {
      new State(stateName[0], {new Transition("loadmap", 1)}),
      new State(stateName[1], {new Transition("loadmap", 1),
                               new Transition("validatemap", 2)}),
      new State(stateName[2], {new Transition("addplayer", 3)}),
      new State(stateName[3], {new Transition("addplayer", 3),
                               new Transition("gamestart", 4)}),
      new State(stateName[4], {new Transition("issueorder", 5)}),
      new State(stateName[5], {new Transition("issueorder", 5),
                               new Transition("endissueorders", 6)}),
      new State(stateName[6],
                {new Transition("execorder", 6),
                 new Transition("endexecorders", 4), new Transition("win", 7)}),
      new State(stateName[7],
                {new Transition("play", 0), new Transition("quit", -1)}),
  };
  _currentStateIndex = 0;
  _nextStateIndex = 0;
  _map = nullptr;
  _players = {};
  deck = new Deck();
  if (fileName == "") {
    _commandProcessor = new CommandProcessor();
    _fileName = "";
  } else {
    _commandProcessor = new FileCommandProcessorAdapter("./config/" + fileName);
    _fileName = fileName;
  }
}

GameEngine::GameEngine(const GameEngine &gameEngine) {
  _state = vector<State *>();
  for (auto const &state : gameEngine._state) {
    _state.push_back(new State(*state));
  }
  _currentStateIndex = gameEngine._currentStateIndex;
  _nextStateIndex = gameEngine._nextStateIndex;
  _map = new Map(*gameEngine._map);
  _players = {};
  for (auto const &player : gameEngine._players)
    _players.push_back(new Player(*player));
  if (deck != nullptr)
      deck = new Deck(*gameEngine.deck);
  _fileName = gameEngine._fileName;

  if (_fileName == "")
    _commandProcessor = new CommandProcessor();

  else
    _commandProcessor =
        new FileCommandProcessorAdapter("./config/" + _fileName);
}

GameEngine::~GameEngine() = default;

GameEngine &GameEngine::operator=(const GameEngine &gameEngine) {
  this->_state = gameEngine._state;
  this->_currentStateIndex = gameEngine._currentStateIndex;
  this->_nextStateIndex = gameEngine._nextStateIndex;
  this->_map = gameEngine._map;
  this->_players = gameEngine._players;
  this->deck = gameEngine.deck;
  return *this;
}

vector<State *> GameEngine::getState() { return _state; }

int GameEngine::getCurrentStateIndex() { return _currentStateIndex; }
int &GameEngine::getNextStateIndex() { return _nextStateIndex; }

CommandProcessor *GameEngine::getCommandProcessor()
{
    return this->_commandProcessor;
}

void GameEngine::setCurrentStateIndex(int currentStateIndex) {
  _currentStateIndex = currentStateIndex;
}

void GameEngine::setNextStateIndex(int nextStateIndex) {
  _nextStateIndex = nextStateIndex;
}

bool GameEngine::transition() {
  string commandOption = "";
  bool isValidCommand = _commandProcessor->validate(
      this->_commandProcessor->getLastCommand(), _currentStateIndex, _nextStateIndex, commandOption);

  if (isValidCommand) {
    const bool valid =
        executeCurrentStateAction(_nextStateIndex, commandOption);
    if (valid) {
        this->_currentStateIndex = _nextStateIndex;
        notify();
    }
    else
      cout << "Invalid. Staying in the same state." << endl;
    return valid;
  }
  return false;
}

bool GameEngine::executeCurrentStateAction(int nextStateIndex, const string &option) {
    string currentStateName = this->_state[this->_currentStateIndex]->getName();
    // Going to mapLoaded
    if (nextStateIndex == 1) {
        delete this->_map;
        this->_map = MapLoader::load(option);
        return true;
    }
    // Going to mapValidated
    if (nextStateIndex == 2) {
        const bool validMap = this->_map->validate();
        if (!validMap) {
            string effect = "Map is not valid. Please try loading another one.";
            cout << effect << endl;
            _commandProcessor->getLastCommand()->saveEffect(effect);
            return false;
        }
        string effect = "Map is valid!";
        cout << effect << endl;
        _commandProcessor->getLastCommand()->saveEffect(effect);
        return true;
    }
    // Going to playersAdded
    if (nextStateIndex == 3) {
        Player *player;
        if (option.empty()) {
            player = new Player();
        } else {
            player = new Player(option, {}, new Hand(), new OrdersList());
        }
        cout << "Player " << player->getName() << " has been added." << endl;
        string effect = "Player " + player->getName() + " has been added.";
        _commandProcessor->getLastCommand()->saveEffect(effect);
        this->_players.push_back(player);
        return true;
    }
    // Going to assignReinforcement: Distribute Territories and Reinforcements: gamestart
    if (nextStateIndex == 4) {
        // The game just started
        if (this->deck == nullptr)
            return this->startGame();
        // Assigning reinforcement - Middle of the Game
        string effect = "Assigning Reinforcements";
        cout << effect << endl;
        _commandProcessor->getLastCommand()->saveEffect(effect);
        this->reinforcementPhase();
        return true;
    }
    // Going to issueOrders
    if (nextStateIndex == 5) {
        string effect = "Issuing Orders";
        cout << effect << endl;
        _commandProcessor->getLastCommand()->saveEffect(effect);
        this->issueOrdersPhase();
        return true;
    }
    // Going to executeOrders
    if (nextStateIndex == 6) {
        string effect = "Executing Orders";
        cout << effect << endl;
        _commandProcessor->getLastCommand()->saveEffect(effect);
        this->executeOrdersPhase();
        return true;
    }
    // Going to win
    if (nextStateIndex == 7) {
        string effect = "Going to win";
        cout << effect << endl;
        _commandProcessor->getLastCommand()->saveEffect(effect);
    }
    string effect = "Something went wrong!";
    _commandProcessor->getLastCommand()->saveEffect(effect);
    return false;
}

bool GameEngine::startGame()
{
    string effect = "Fairly distributing the territories to the players";
    cout << effect << endl;
    _commandProcessor->getLastCommand()->saveEffect(effect);

    // Check if there are 2 to 6 players
    if (this->_players.size() < 2 || this->_players.size() > 6)
    {
        cout << "The players are not in the specified range: 2-6." << endl;
        return false;
    }

    // Create a new deck
    delete this->deck;
    this->deck = new Deck();

    // 4-a fairly distribute all the territories to the players

    // Shuffling the territories
    vector<Territory *> shuffledTerritories = this->_map->getTerritories();
    auto rng = std::default_random_engine{};
    std::shuffle(std::begin(shuffledTerritories), std::end(shuffledTerritories),
                 rng);
    int numberOfPlayer = this->_players.size();
    int count = 0;

    // Loop through players and add territories
    for (auto const &territory : shuffledTerritories) {
        Player *player = this->_players[count++];
        player->addOwnedTerritory(territory);
        territory->setOwnedBy(player, 0);
        // When reaching the end, repeat
        if (count == numberOfPlayer) {
            count = 0;
        }
    }
    // 4-b determine randomly the order of play of the players in the game
    cout << "Shuffling players..." << endl;
    rng = std::default_random_engine{std::random_device{}()};
    std::shuffle(std::begin(this->_players), std::end(this->_players), rng);
    cout << "Shuffled player order:" << endl;
    count = 0;
    for (auto const player : this->_players) {
        cout << "\t" << std::to_string(count++) << ": " << player->getName()
             << endl;

        // 4-c give 50 initial army units to the players, which are placed in
        // their respective reinforcement pool
        player->setArmyUnits(50);
        cout << "\t\tSetting " << player->getArmyUnits() << " army units."
             << endl;

        // 4-d let each player draw 2 initial cards from the deck using the
        // deck’s draw() method
        Card *card1 = this->deck->draw();
        player->getPlayerHand()->addCard(card1);
        cout << "\t\tDraw Card 1: " << card1->getCardType() << endl;
        Card *card2 = this->deck->draw();
        player->getPlayerHand()->addCard(card2);
        cout << "\t\tDraw Card 2: " << card2->getCardType() << endl;
    }
    return true;
}

void GameEngine::reinforcementPhase()
{
    for (Player* player : this->_players)
    {
        cout << player->getName() << " owns " << player->getOwnedTerritories().size() << " territories." << endl;

        int armyUnits = (player->getOwnedTerritories().size() / 3);
        if (player->getArmyUnits() > 0)
            armyUnits += player->getArmyUnits();

        if (armyUnits < 3)
            player->setArmyUnits(3);
        else
        {
            for (Continent* c : this->_map->getContinents())
            {
                bool ownsContinent = false;
                for (Territory* t : this->_map->getTerritories())
                {
                    if (c->getName() == t->getContinent()->getName())
                    {
                        for (Territory* ownedTerritory : player->getOwnedTerritories())
                        {
                            ownsContinent = ownedTerritory->getName() == t->getName();
                            if (ownsContinent)
                                break;
                        }
                    }
                }
                if (ownsContinent)
                {
                    cout << player->getName() << " owns continent " << c->getName() << " and gained " << c->getScore() << " army units." << endl;
                    armyUnits = armyUnits + c->getScore();
                }
                else
                    cout << player->getName() << " does not own continent " << c->getName() << "." << endl;
            }
            player->setArmyUnits(armyUnits);
        }
        cout << player->getName() << " has " << player->getArmyUnits() << " army units this turn." << endl;
        cout << endl;
    }
}

bool GameEngine::allPlayerCardsPlayed() const
{
    bool allCardsPlayed = true;
    for (Player* p : this->_players)
    {
        cout << p->getName() << ": " << p->getPlayerHand()->cards.size() << " cards." << endl;
        allCardsPlayed = allCardsPlayed && (p->getPlayerHand()->cards.size() != 0);
    }
    return allCardsPlayed;
}

//void GameEngine::issueOrdersPhase()
//{
//    OrdersList* allIssuedOrders = new OrdersList();
//    while (allPlayerCardsPlayed())
//    {
//        for (Player* p : this->_players)
//        {
//            // 5 is used here to keep a few territories to attack to be used by Cards
//            if (p->getAttackList().size() == 5 && p->getPlayerHand()->cards.size() != 0)
//            {
//                Order* o = p->getPlayerHand()->cards.at(0)->play(p, this->deck);
//                if (o != nullptr)
//                    allIssuedOrders->addOrder(o);
//            }
//            else
//            {
//                Order* o = p->issueOrder();
//                if (o != nullptr)
//                    allIssuedOrders->addOrder(o);
//            }
//        }
//    }
//    this->ordersList = allIssuedOrders;
//
//    // Clear all players toAttack and toDefend lists
//    for (Player* p : this->_players)
//    {
//        p->setAttackList(vector<Territory*>());
//        p->setDefendList(vector<Territory*>());
//    }
//}

void GameEngine::issueOrdersPhase()
{
    bool allPlayersDone = false;
    OrdersList* allIssuedOrders = new OrdersList();
    //OrdersList* list = new OrdersList();
    vector<Order*> list;
    while (!allPlayersDone)
    {
        for (Player* p : this->_players)
        {
            Order* o = p->issueOrder();
            //list->addOrder(o);
            list.push_back(o);
            if (o != nullptr)
            {
                allIssuedOrders->addOrder(o);
            }
        }

        for (int i = 0; i < this->_players.size(); i++)
        {
            // Players cannot finish issuing orders until all armies are deployed
            if (_players.at(i)->getArmyUnits() != 0)
            {
                allPlayersDone = false;
                break;
            }
            else {
                // Check the end of the list, if the last #ofplayers orders are null, 
                // then all players are done issuing orders
                // If any of the last #ofplayers order are not null (advance, card, etc.) then
                // not all players are done
                if (list.at(list.size() - 1 - i))
                {
                    allPlayersDone = false;
                    break;
                }
                else
                {
                    allPlayersDone = true;
                }
            }
        }

    }
    this->ordersList = allIssuedOrders;

    // Clear all players toAttack and toDefend lists
    for (Player* p : this->_players)
    {
        p->setAttackList(vector<Territory*>());
        p->setDefendList(vector<Territory*>());
    }
}


void GameEngine::executeOrdersPhase()
{
    for (Order* o : this->ordersList->getOrdersList())
        o->execute();
    this->ordersList = nullptr;
}

int GameEngine::validateGameRound()
{
    for (int i = 0; i < this->_players.size(); i++)
    {
        Player* player = this->_players[i];
        int ownedTerritoryCount = player->getOwnedTerritories().size();
        // Kick out player that do not have any territories
        if (ownedTerritoryCount == 0)
        {
            cout << player->getName() << " does not own at least one territory." << endl
                 << player->getName() << " is removed from the game." << endl << endl;
            this->_players.erase(this->_players.begin() + i);
            continue;
        }
        // Check if player owns all territories
        if (ownedTerritoryCount == this->_map->getTerritories().size())
        {
            cout << player->getName() << " owns all the territories." << endl << endl;
            return i;
        }
    }
    return -1;
}

void GameEngine::mainGameLoop()
{
    int winnerIndex = validateGameRound();
    vector<int> ownedTerritory = getOwnedTerritories({});
    while (winnerIndex == -1)
    {
        cout << endl
             << "------------- Issue Orders Phase -------------" << endl
             << endl;
        this->_commandProcessor->saveCommand("issueorder");
        this->transition();
        cout << endl
             << "------------- Execute Orders Phase -------------" << endl
             << endl;
        this->_commandProcessor->saveCommand("endissueorders");
        this->transition();
        cout << endl
             << "------------- Reinforcement Phase -------------" << endl
             << endl;
        this->_commandProcessor->saveCommand("endexecorders");
        this->transition();

        ownedTerritory = getOwnedTerritories(ownedTerritory);

        winnerIndex = validateGameRound();
    }
    cout << this->_players[winnerIndex]->getName() << " is the winner." << endl;
}

vector<int> GameEngine::getOwnedTerritories(vector<int> ownedTerritory)
{
    vector<int> newOwnedTerritory = {};
    for (int i = 0; i < this->_players.size(); i++)
    {
        newOwnedTerritory.push_back(this->_players[i]->getOwnedTerritories().size());
        if (!ownedTerritory.empty())
        {
            int diff = this->_players[i]->getOwnedTerritories().size() - ownedTerritory[i];
            cout << ownedTerritory[i];
            if (diff > 0) {
                cout << this->_players[i]->getName() << " needs to draw " << diff << " cards." << endl;
                for (int t = 0; t < diff; t++) {
                    Card *card = this->deck->draw();
                    this->_players[i]->getPlayerHand()->addCard(card);
                    cout << "\t\tDraw Card: " << card->getCardType() << endl;
                }
                cout << endl;
            }
        }
    }
    return newOwnedTerritory;
}

//Print a list of all states with their valid transitions
void GameEngine::startupPhase()
{
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

  // If the next index is -1, the game will terminate.
  while (this->_currentStateIndex != -1) {

    cout << *this->getState()[this->getCurrentStateIndex()];
    cout << ">> ";

    Command *commandObj = _commandProcessor->getCommand();

    // Used as a flag to be true if the command is valid to allow going to the next state
    bool validCommand = this->transition();

    // If the user command is invalid, print an error message
    if (!validCommand)
      cout << endl
           << "-_- Your command: \"" << commandObj->getUserCommand()
           << "\" is invalid for the \""
           << this->_state[this->_currentStateIndex]->getName() +
                  "\" state. -_-"
           << endl;

    // 4-e switch the game to the play phase
    if (validCommand && commandObj->getUserCommand() == "gamestart")
    {
        cout << endl << "Play phase..." << endl;
        break;
    }

    cout << endl;
  }
}
std::string GameEngine::stringToLog() {
    return "GameEngine::transition(): " + this->_state[this->_currentStateIndex]->getName() + "\n";
}

Deck* GameEngine::getDeck()
{
    return this->deck;
}

// Print a list of all states with their valid transitions
ostream &operator<<(ostream &strm, const GameEngine &gameEngine) {
  for (auto const &state : gameEngine._state) {
    strm << *state << endl;
  }
  return strm;
}
