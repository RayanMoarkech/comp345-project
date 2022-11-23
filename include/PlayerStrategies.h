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
private:
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

