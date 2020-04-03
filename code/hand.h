#ifndef HAND_H
#define HAND_H

#include <vector>
#include "card.h"

class Hand
{
  friend class Dealer;

  public:
   Hand(std::vector<Card> hand);
   void modify_hand(std::vector <Card> mod_hand);
   int calc_value(std::vector <Card> _hand);
   int find_high_card(std::vector <Card> _hand);

  private:
   int hand_value;            //value of hand
   std::vector <Card> _hand;  //vector with all 5 cards player is holding
};

#endif
