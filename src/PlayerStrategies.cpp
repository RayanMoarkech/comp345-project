//
// COMP345_PROJECT_PLAYERSTRATEGIES_CPP PlayerStrategies.CPP
//

#include "../include/PlayerStrategies.h"

#include <iostream>
    using std::cout;
    using std::endl;


// ---------------------------------------------
// ---------- PlayerStrategy Section -----------
// ---------------------------------------------

// Constructors

PlayerStrategy::PlayerStrategy()
{
    this->_player = nullptr;
}

PlayerStrategy::PlayerStrategy(Player *player)
{
    this->_player = player;
}

PlayerStrategy::PlayerStrategy(const PlayerStrategy &playerStrategy)
{
    this->_player = playerStrategy._player;
}

// Getters and Setters

void PlayerStrategy::setPlayer(Player *player)
{
    this->_player = player;
}

Player *PlayerStrategy::getPlayer()
{
    return this->_player;
}

// Overloading

ostream& operator<<(ostream& os, PlayerStrategy& playerStrategy)
{
    cout << "PlayerStrategy with ";
    if (playerStrategy._player)
        cout << "player " << playerStrategy._player << endl;
    else
        cout << "no player" << endl;
    return cout;
}

PlayerStrategy &PlayerStrategy::operator=(const PlayerStrategy &playerStrategy) = default;

// Destructor

PlayerStrategy::~PlayerStrategy() = default;


// ---------------------------------------------
// ------- NeutralPlayerStrategy Section -------
// ---------------------------------------------

// Constructors

NeutralPlayerStrategy::NeutralPlayerStrategy(): PlayerStrategy() {}

NeutralPlayerStrategy::NeutralPlayerStrategy(Player *player): PlayerStrategy(player) {}

NeutralPlayerStrategy::NeutralPlayerStrategy(const PlayerStrategy &playerStrategy): PlayerStrategy(playerStrategy) {}

// Functionalities

// never issues any order
PlayerStrategy* NeutralPlayerStrategy::issueOrder()
{
    return this;
}

PlayerStrategy* NeutralPlayerStrategy::toAttack()
{
    // TODO: not sure what to put here
    return this;
}

// is attacked, it becomes an Aggressive player
PlayerStrategy* NeutralPlayerStrategy::toDefend()
{
    return new AggressivePlayerStrategy(*this);
}

// Destructor

NeutralPlayerStrategy::~NeutralPlayerStrategy() = default;

