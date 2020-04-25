#ifndef DEALER_GAME_H
#define DEALER_GAME_H

#include "pot.h"
#include "deck.h"
#include "json.hpp"
#include "chat_message.hpp"
#include <vector>
#include <string>

class Dealer_Game 
{
  public:
    Dealer_Game(Deck deck);
    void shuffleCards();
    chat_message dealCards(int num);
    bool gameResult();
    chat_message exchangeCards(int amountOfCards);
    void revealHand();
    void addMoney(int amount);

  private:
    bool _playerResult;
    Deck _deck;
    Pot _pot{0};
};

#endif
