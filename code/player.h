#ifndef PLAYER_H
#define PLAYER_H

#include "hand.h"
#include "stack.h"


class Player
{
   Hand _hand;

  public:
   Stack _stack;
   std::string _name;
   
};

#endif
