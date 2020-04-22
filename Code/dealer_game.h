#ifndef DEALER_GAME_H
#define DEALER_GAME_H

#include "chip.h"
#include "card.h"
#include "deck.h"
#include "json.hpp"
#include "chat_message.hpp"
#include <vector>
#include <string>
#include <deque>

class Dealer_Game 
{
  public:
    Dealer_Game(bool playerResult);
    void shuffleCards();
    chat_message dealCards();
    void dealChips();
    bool gameResult();
    void exchangeCards(int amountOfCards);
    void revealHand();

  private:
    bool _playerResult;
    Deck _deck;
    std::vector<Chip> chips;
};

#endif
