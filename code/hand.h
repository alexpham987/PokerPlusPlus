#ifndef HAND_H
#define HAND_H

#include <vector>
#include "card.h"

class Hand
{
   int hand_value;            //value of hand
   std::vector <Card> _hand;  //vector with all 5 cards player is holding
   void modify_hand (std::vector <Card> mod_hand);
   int calc_value(std::vector <Card> _hand);
   int find_high_card(std::vector <Card> _hand);

  public:

   Hand( std::vector < Card >);
   ~Hand();

   friend class Dealer;
};

#endif
   
