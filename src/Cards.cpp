#include "../include/Cards.h"
#include "../include/Player.h"
#include <ostream>
#include <cstdlib>

// ---------------------------------------------
// --------------- Card Class ------------------
// ---------------------------------------------

// Default constructor
Card::Card() {
    switch (rand() % 5) {
        case 0:
            type = BOMB;
            break;
        case 1:
            type = REINFORCEMENT;
            break;
        case 2:
            type = BLOCKADE;
            break;
        case 3:
            type = AIRLIFT;
            break;
        case 4:
            type = DIPLOMACY;
            break;
    }
}

// Parameterized constructor with Card type
Card::Card(CardType t) {
    type = t;
}

// Copy constructor
Card::Card(const Card &card) {
    type = card.type;
}

// Destructor
Card::~Card() = default;

// Assignment operator
Card& Card::operator = (const Card& card) {
    this->type = card.type;
    return *this;
}

// Stream insertion operator
ostream& operator << (ostream& out, const Card& card) {
    switch (card.type) {
        case BOMB:
            out << "Bomb";
            break;
        case REINFORCEMENT:
            out << "Reinforcement";
            break;
        case BLOCKADE:
            out << "Blockade";
            break;
        case AIRLIFT:
            out << "Airlift";
            break;
        case DIPLOMACY:
            out << "Diplomacy";
            break;
    }
    return out;
}

// Play a card
void Card::play(Player* player, Deck* deck)
{
    switch (this->type ) {
        case BOMB:
            cout << "Card played: BOMB\n";
            player->issueOrder();
            break;
        case REINFORCEMENT:
            cout << "Card played: REINFORCEMENT\n";
            player->issueOrder();
            break;
        case BLOCKADE:
            cout << "Card played: BLOCKADE\n";
            player->issueOrder();
            break;
        case AIRLIFT:
            cout << "Card played: AIRLIFT\n";
            player->issueOrder();
            break;
        case DIPLOMACY:
            cout << "Card played: DIPLOMACY\n";
            player->issueOrder();
            break;
    }
    deck->returnCard(this);
}

// ---------------------------------------------
// --------------- Deck Class ------------------
// ---------------------------------------------

// Default constructor
Deck::Deck() {
    for (int i = 0; i < DECK_SIZE; i++) {
        cards.push_back(new Card());
    }
}

// Copy constructor
Deck::Deck(const Deck& deck) {
    this->cards = deck.cards;
}

// Destructor
Deck::~Deck() {
    for (auto & card : cards) {
        delete card;
    }
    cards.clear();
}

// Assignment operator
Deck& Deck::operator = (const Deck& deck) {
    this->cards = deck.cards;
    return *this;
}

// Stream insertion operator
ostream& operator << (ostream& out, const Deck& deck) {
    for (auto & card : deck.cards) {
        out << *card << endl;
    }
    return out;
}

// Draw a card from the deck
Card* Deck::draw() {
    int randomIndex = rand() % cards.size();
    Card* card = cards.at(randomIndex);
    cards.erase(cards.begin() + randomIndex);
    return card;
}

// Return card to the deck
void Deck::returnCard(Card* card) {
    cards.push_back(card);
}

// ---------------------------------------------
// --------------- Hand Class ------------------
// ---------------------------------------------

// Default constructor
Hand::Hand() = default;

// Copy constructor
Hand::Hand(const Hand& hand) {
    this->cards = hand.cards;
}

// Destructor
Hand::~Hand() {
    for (auto & card : cards) {
        delete card;
    } //Freeing each card's memory then clearing the Hand's vector
    cards.clear();
}

// Assignment operator
Hand& Hand::operator = (const Hand& hand) {
    this->cards = hand.cards;
    return *this;
}

// Stream insertion operator
ostream& operator << (ostream& out, const Hand& hand) {
    for (auto & card : hand.cards) {
        out << *card << endl; // Double check
    }
    return out;
}

// Add card to the hand
void Hand::addCard(Card* card) {
    cards.push_back(card);
}