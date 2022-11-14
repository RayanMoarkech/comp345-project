#ifndef COMP345_CARDS_H
#define COMP345_CARDS_H

#include <iostream>
using std::cout;
using std::endl;
using std::ostream;
#include <vector>
using std::vector;

#define DECK_SIZE 54
#define HAND_SIZE 5



class Deck;
class Hand;
class Player;
class Order;

// ---------------------------------------------
// ---------------- Card Section ---------------
// ---------------------------------------------
enum CardType {BOMB, REINFORCEMENT, BLOCKADE, AIRLIFT, DIPLOMACY }; // Possible types of cards

class Card {
private:
    CardType type; // Type of the card

public:
    Card(); // Default constructor

    Card(CardType t); // Parameterized constructor with Card type

    Card(const Card &card); // Copy constructor

    ~Card(); // Destructor

    std::string getCardType(); // Returns the type of the card

    Card &operator=(const Card &card); // Assignment operator overloading

    friend ostream &operator<<(ostream &out, const Card &card); // Stream insertion operator overloading

    Order* play(Player *player, Deck *deck); // Play a card
};

// ---------------------------------------------
// ---------------- Deck Section ---------------
// ---------------------------------------------

class Deck {
    private:
    std::vector<Card*> cards; // Vector of the cards in the deck
public:

    Deck(); // Default constructor

    Deck(const Deck& deck); // Copy constructor

    ~Deck(); // Destructor

    Deck& operator=(const Deck& deck); // Assignment operator overloading

    friend ostream& operator<<(ostream& out, const Deck& deck); // Stream insertion operator overloading

    Card* draw(); // Draw a card from the deck

    void returnCard(Card* card); // Return a card to the deck

    vector<Card *> getDeck(); // Return the deck

};

// ---------------------------------------------
// ---------------- Hand Section ---------------
// ---------------------------------------------

class Hand {
public:
    std::vector<Card*> cards; // Vector of the cards in the hand
    Hand(); // Default constructor

    Hand(const Hand& hand); // Copy constructor

    ~Hand(); // Destructor

    Hand& operator=(const Hand& hand); // Assignment operator overloading

    friend ostream& operator<<(ostream& out, const Hand& hand); // Stream insertion operator overloading

    void addCard(Card* card); // Add a card to the hand
};

#endif //COMP345_CARDS_H