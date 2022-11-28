//
// COMP345_PROJECT_PLAYERSTRATEGIES_H PlayerStrategies.h
//

#ifndef PLAYERSTRATEGIES_H
#define PLAYERSTRATEGIES_H

#include <ostream>
    using std::ostream;

class Player;


// ---------------------------------------------
// ---------- PlayerStrategy Section -----------
// ---------------------------------------------

class PlayerStrategy {
protected:
    Player* _player;
public:

    PlayerStrategy();
    PlayerStrategy(Player* player);
    PlayerStrategy(const PlayerStrategy& playerStrategy);

    void setPlayer(Player* player);
    Player* getPlayer();

    virtual PlayerStrategy* issueOrder() = 0;
    virtual PlayerStrategy* toAttack() = 0;
    virtual PlayerStrategy* toDefend() = 0;

    friend ostream& operator<<(ostream& os, PlayerStrategy& playerStrategy);
    PlayerStrategy& operator=(const PlayerStrategy& playerStrategy);

    ~PlayerStrategy();
};


// ---------------------------------------------
// ------- NeutralPlayerStrategy Section -------
// ---------------------------------------------

class NeutralPlayerStrategy: public PlayerStrategy {
public:
    NeutralPlayerStrategy();
    NeutralPlayerStrategy(Player* player);
    NeutralPlayerStrategy(const PlayerStrategy& playerStrategy);

    PlayerStrategy* issueOrder();
    PlayerStrategy* toAttack();
    PlayerStrategy* toDefend();

//    friend ostream& operator<<(ostream& os, PlayerStrategies& playerStrategies);
//    PlayerStrategies& operator=(const PlayerStrategies& playerStrategies);

    ~NeutralPlayerStrategy();
};

// ---------------------------------------------
// ------BenevolentPlayerStrategy Section-------
// ---------------------------------------------

class BenevolentPlayerStrategy : public PlayerStrategy {
public:
    BenevolentPlayerStrategy();
    BenevolentPlayerStrategy(Player* player);
    BenevolentPlayerStrategy(const PlayerStrategy& playerStrategy);

    PlayerStrategy* issueOrder();
    PlayerStrategy* toAttack();
    PlayerStrategy* toDefend();

    //    friend ostream& operator<<(ostream& os, PlayerStrategies& playerStrategies);
    //    PlayerStrategies& operator=(const PlayerStrategies& playerStrategies);

    int toDefendIndex = 0;
    int toAdvanceIndex = 0;

    ~BenevolentPlayerStrategy();
};


// ---------------------------------------------
// ----- AggressivePlayerStrategy Section ------
// ---------------------------------------------

class AggressivePlayerStrategy: public PlayerStrategy {
public:
    AggressivePlayerStrategy();
    AggressivePlayerStrategy(Player* player);
    AggressivePlayerStrategy(const PlayerStrategy& playerStrategy);

    PlayerStrategy* issueOrder();
    PlayerStrategy* toAttack();
    PlayerStrategy* toDefend();

//    friend ostream& operator<<(ostream& os, PlayerStrategies& playerStrategies);
//    PlayerStrategies& operator=(const PlayerStrategies& playerStrategies);

    ~AggressivePlayerStrategy();
};

// ---------------------------------------------
// ----- HumanPlayerStrategy Section ------
// ---------------------------------------------

class HumanPlayerStrategy : public PlayerStrategy {
public:
    HumanPlayerStrategy();
    HumanPlayerStrategy(Player* player);
    HumanPlayerStrategy(const PlayerStrategy& playerStrategy);

    PlayerStrategy* issueOrder();
    PlayerStrategy* toAttack();
    PlayerStrategy* toDefend();

    //    friend ostream& operator<<(ostream& os, PlayerStrategies& playerStrategies);
    //    PlayerStrategies& operator=(const PlayerStrategies& playerStrategies);

    ~HumanPlayerStrategy();
};

#endif //PLAYERSTRATEGIES_H
