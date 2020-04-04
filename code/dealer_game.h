#ifndef DEALER_GAME_H
#define DEALER_GAME_H

#include "chip.h"
#include "card.h"
#include "deck.h"
//#include "player.h"
#include <vector>
#include <string>
#include <deque>

class Dealer_Game
{
  public:
    Dealer_Game(bool playerResult);
    void shuffleCards();
    void dealCards();
    void dealChips();
    bool gameResult();
    void exchangeCards(int amountOfCards);
    void revealHand();

  private:
    bool _playerResult;
    std::vector<Card> deck;
    std::vector<Chip> chips;
    //std::deque<Player> players;
};

#endif
