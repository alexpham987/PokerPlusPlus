#ifndef PLAYER_H
#define PLAYER_H

#include "hand.h"
#include "stack.h"
#include "json.hpp"
#include <boost/uuid/uuid.hpp>  
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp> 


class Player
{
   //Hand _hand;

  public:

   Player();
   Stack _stack;
   std::string _name;
   boost::uuids::uuid id;

   nlohmann::json move_j() const; // method to be used by dealer to find out player move 

   friend class Dealer;
};


#endif