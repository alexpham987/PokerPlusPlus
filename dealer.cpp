#include "dealer.h"

Dealer::Dealer(Deck deck)
{}
void Dealer::shuffleCards(Deck deck)
{}
void Dealer::dealCards()
{}
void Dealer::dealChips()
{}
bool Dealer::gameResult()
{
  if(playerWin == true)
  {
    return true;
  }
  else
  {
    return false;
  }
}
void dealerTransmission(std::string json)
{}
