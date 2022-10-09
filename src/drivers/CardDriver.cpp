#include <iostream>
#include <vector>
#include "../../include/Cards.h"
#include "../../include//Player.h"

using namespace std;

void testCards() {
    // change seed to get random numbers every run
    srand(time(NULL));

    cout << "Creating player and deck object..." << endl;
    auto* player = new Player();
    Deck* deck = new Deck();
    cout << "Player and Deck successfully created!" << endl;

    // Status Summary
    cout << "\nThe following deck consists of:" << endl;
    cout << *deck;
    cout << "\nPlayer Hand Size: " << player->playerHand->cards.size() << endl;

    // Fill players hand with cards
    cout << endl << "\nDrawing cards to fill the hand..." << endl;
    for (int i = 0; i < HAND_SIZE; i++) {
        Card* card = deck->draw();
        cout << "Card "  << i << ": " << *card << endl;
        player->playerHand->addCard(card);
    }

    // Status Summary
    cout << endl << "\nHand Has been Completed!" << endl;
    cout << "Player Hand Size: " << player->playerHand->cards.size() << endl;
    cout << "Deck Size: " << deck->cards.size() << endl;


    // Play each card in the hand
    cout << endl << "Player Hand: \n" << *player->playerHand << endl;
    cout << endl << "Playing each Card...\n" << endl;

    while (!player->playerHand->cards.empty()) {
            Card* card = player->playerHand->cards.front();
            card->play(player, deck);
        }

    cout << endl << "\nAll cards have been played in the hand!\n" << endl;
    cout << "Player Hand Size: " << player->playerHand->cards.size() << endl;
    cout << "Deck Size: " << deck->cards.size() << endl;

    // Freeing the memory
    delete deck;
    delete player;
    cout << "Player and Deck successfully deleted!" << endl;

}