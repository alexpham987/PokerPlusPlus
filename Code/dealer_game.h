#ifndef DEALER_GAME_H
#define DEALER_GAME_H

#include "chip.h"
#include "card.h"
#include "deck.h"
#include "dealer_comm.h"
#include <vector>
#include <string>
#include <deque>

class Dealer_Game 
{
  public:
    Dealer_Game(bool playerResult);
    void shuffleCards();
    void dealCards(chat_room& c);
    void dealChips();
    bool gameResult();
    void exchangeCards(int amountOfCards);
    void revealHand();

  private:
    bool _playerResult;
    Deck deck;
    std::vector<Chip> chips;
};

#endif
