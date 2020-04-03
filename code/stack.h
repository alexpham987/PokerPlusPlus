#ifndef STACK_H
#define STACK_H

#include <vector>
#include "chip.h"

class Stack
{
  public:
    void add_chips (int value_chips_added);  //adds chips to player's stack
    void remove_chips (int value_chips_removed); //removes chips from player's stack

   friend class Dealer;

  private:
    int total;                  //total value of player's chips
    std::vector <Chip> _stack;  //vector with all the player's chips
};


#endif
