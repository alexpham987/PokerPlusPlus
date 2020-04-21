
#ifndef HAND_H
#define HAND_H

//#include <vector>
#include <algorithm>
#include "card.h"

class Hand
{
  friend class Dealer;
  
  public:
   Hand();
   ~Hand();
   //bool comp_suit(const Card & a, const Card & b);
   //bool comp_value(const Card & a, const Card & b);
   void modify_hand ();
   int calc_value();
   int find_high_card();

  private:
   int hand_value;            //value of hand
   std::vector <Card> _hand;
};

#endif
