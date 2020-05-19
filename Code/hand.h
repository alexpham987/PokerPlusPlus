#ifndef HAND_H
#define HAND_H

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
    int find_next_highest();
    void addCard(Card c);
    std::vector<Card> getHand();

  private:
    int hand_value; //attribute that represents the value of a hand
    std::vector<Card> _hand; //attribute that represents the hand itself
};

#endif
