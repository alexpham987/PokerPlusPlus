#ifndef PLAYER_H
#define PLAYER_H

#include "hand.h"
#include "stack.h"
#include "json.hpp"


class Player
{
   Hand _hand;

  public:

   Stack _stack;
   std::string _name;
   int id;

   nlohmann::json move_j() const; // method to be used by dealer to find out player move 

   friend class Dealer;
};


#endif