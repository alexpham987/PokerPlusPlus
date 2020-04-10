#ifndef PLAYER_GAME_H
#define PLAYER_GAME_H

#include "hand.h"
#include "stack.h"
#include "json.hpp"
#include "player_comm.h"
#include "dealer_game.h"
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
   int current_bet;
   int total_bet;
   int cards_requested;
   //std::string chat;

   nlohmann::json move_j(std::string play, std::string event, int cards_requested, int total_bet) const; // method to be used by dealer to find out player move

   friend class player_comm;
   friend class Dealer_game;
};

#endif
