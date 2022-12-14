#include "../include/Cards.h"
#include "../include/Player.h"

#include <iostream>
	using std::cout;
#include <cstdlib>

// ---------------------------------------------
// --------------- Card Class ------------------
// ---------------------------------------------

// Default constructor
Card::Card() {
		this->deck = nullptr;
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

// Outputs Card type as a string
std::string Card::getCardType() {
    switch(type) {
        case CardType::AIRLIFT:         return "Airlift";
        case CardType::BLOCKADE:        return "Blockade";
        case CardType::BOMB:            return "Bomb";
        case CardType::DIPLOMACY:       return "Diplomacy";
        case CardType::REINFORCEMENT:   return "Reinforcement";
        default:                        return "Error: Unable to determine card type";
    }
}

// Set the Deck that the card belongs to
void Card::setDeck(Deck *deck)
{
		this->deck = deck;
}

Deck *Card::getDeck()
{
		return this->deck;
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
Order* Card::play(Player* player, Deck* deck)
{
    Order* cardOrder = player->issueOrder();
    deck->returnCard(this);
    player->getPlayerHand()->cards.erase(player->getPlayerHand()->cards.begin());
    return cardOrder;
}

// ---------------------------------------------
// --------------- Deck Class ------------------
// ---------------------------------------------

// Default constructor
Deck::Deck() {
    for (int i = 0; i < DECK_SIZE; i++) {
				Card* newCard = new Card();
				newCard->setDeck(this);
        cards.push_back(newCard);
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
		if (cards.empty())
			return nullptr;
    int randomIndex = rand() % cards.size();
    Card* card = cards.at(randomIndex);
    cards.erase(cards.begin() + randomIndex);
		card->setDeck(this);
    return card;
}

vector<Card *> Deck::getDeck() {
    return this->cards;
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
