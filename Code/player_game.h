#ifndef PLAYER_GAME_H
#define PLAYER_GAME_H

#include "hand.h"
#include "stack.h"
#include "json.hpp"
#include "player_comm.h"
#include "dealer_game.h"
#include <vector>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>


class Player_Game
{

  public:
   Player_Game(player_comm* pc);
   void setName(std::string name);
   nlohmann::json move_j(std::string play, int cards_requested, int current_bet) const;
   void exchange_j(std::string play, int cards_requested, std::vector<int> cards);

  private:
   std::string _name;
   boost::uuids::uuid _id;
   Stack _stack;
   Hand _hand;
   player_comm* _pc;

   friend class Dealer_game;
};

#endif
