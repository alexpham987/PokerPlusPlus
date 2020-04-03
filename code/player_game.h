#ifndef PLAYER_GAME_H
#define PLAYER_GAME_H

#include "hand.h"
#include "stack.h"
#include "json.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>


class Player_Game
{
   //Hand _hand;

  public:

   Player_Game();
   Stack _stack;
   std::string _name;
   boost::uuids::uuid id;

   //nlohmann::json move_j() const; // method to be used by dealer to find out player move

   //friend class Dealer;
};

#endif
