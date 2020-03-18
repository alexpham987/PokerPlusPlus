#ifndef DEALER_H
#define DEALER_H

class Dealer
{
  public:
    Dealer(/*Deck deckOfCards*/);
    void dealCards();
    void dealChips();
    bool gameResult(/*something here?*/);

  private:
    bool playerWin;
};

#endif
