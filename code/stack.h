#ifndef STACK_H
#define STACK_H

#include <vector>
#include "chip.h"

class Stack
{
  public:
    
    int total;                  //total value of player's chips
    std::vector <Chip> _stack;  //vector with all the player's chips
    void add_chips (int value_chips_added);  //adds chips to player's stack
    void remove_chips (int value_chips_removed); //removes chips from player's stack
    

};


#endif
