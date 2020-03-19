#ifndef STACK_H
#define STACK_H

#include <vector>
#include "chip.h"

class Stack
{
  public:
    
    int total;                  //total value of player's chips
    std::vector <Chip> _stack;  //vector with all the player's chips

}

