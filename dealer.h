#ifndef DEALER_H
#define DEALER_H

//#include "deck.h"

class Dealer
{
  public:
    Dealer(Deck deck);
    void shuffleCards(Deck deck);
    void dealCards();
    void dealChips();
    bool gameResult();
    void dealerTransmission(std::string json);

  private:
    bool playerWin;
};

#endif
