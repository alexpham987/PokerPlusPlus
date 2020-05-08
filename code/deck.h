#ifndef __DECK_H
#define __DECK_H

#include "card.h"
#include <vector>
#include <stdexcept>
#include <algorithm>

class Deck
{
  public:
    Deck();
    Card deal();
    void shuffle();
    int remaining_cards();
    bool deck_empty();

  private:
    std::vector<Card> _deck;  //attribute that represents the deck of cards
};

#endif
