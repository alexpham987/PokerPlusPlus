#ifndef DEALER_H
#define DEALER_H

#include "chip.h"
#include "card.h"
#include "deck.h"
#include "player.h"
#include "asio.hpp"
#include <vector>
#include <string>

using asio::ip::tcp;

class Dealer
{
  public:
    Dealer(bool playerResult();
    void shuffleCards();
    void dealCards();
    void dealChips();
    bool gameResult();

    //dealer communication to player below
    void playerJoin(Player player);
    void playerLeave(Player player);
    void deliverMessage(std::string message);


  private:
    bool _playerResult;
    std::vector<Card> deck;
    std::vector<Chip> chips;
    std::vector<Player> players;
};

#endif
