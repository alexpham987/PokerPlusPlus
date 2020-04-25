#ifndef HAND_H
#define HAND_H

//#include <vector>
#include <algorithm>
#include <vector>
#include "card.h"

class Hand
{

  public:
   Hand();
   ~Hand();
   //bool comp_suit(const Card & a, const Card & b);
   //bool comp_value(const Card & a, const Card & b);
   void modify_hand (std::vector<int> cards);
   int calc_value();
   int find_high_card();
   void addCard(Card c);
   std::vector<Card> getHand();

  private:
   int hand_value;            //value of hand
   std::vector <Card> _hand;


   friend class Dealer;
};

#endif
   
