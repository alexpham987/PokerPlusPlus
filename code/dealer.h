#ifndef DEALER_H
#define DEALER_H

#include "chip.h"
#include "card.h"
#include "deck.h"
#include "player.h"
//#include asio
#include <vector>
#include <string>
#include <deque>

//using asio::ip::tcp;

class Dealer
{
  public:
    Dealer(bool playerResult);
    void shuffleCards();
    void dealCards();
    void dealChips();
    bool gameResult();
    void exchangeCards(int amountOfCards);
    void revealHand();

    //dealer communication to player below
    void playerJoin(Player player);
    void playerLeave(Player player);
    void deliverMessage(std::string message);


  private:
    bool _playerResult;
    std::vector<Card> deck;
    std::vector<Chip> chips;
    std::deque<Player> players;
};

#endif
