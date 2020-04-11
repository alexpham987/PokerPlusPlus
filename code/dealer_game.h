#ifndef DEALER_GAME_H
#define DEALER_GAME_H

#include "chip.h"
#include "card.h"
#include "deck.h"
#include "stack.h"
#include "json.hpp"
#include "algorithm"
#include "random"
#include <vector>
#include <string>

class Dealer_Game
{
  friend class Deck;
  friend class Stack;

  public:
    Dealer_Game(bool playerResult, Deck d, Stack s);
    void shuffleCards();
    void dealCards();
    void dealChips();
    bool gameResult();
    void exchangeCards(int amountOfCards);
    void revealHand();

  private:
    bool _playerResult;
    std::random_device random;
    Deck _d;
    Stack _s;
};

#endif
