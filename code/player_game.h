#ifndef PLAYER_GAME_H
#define PLAYER_GAME_H

#include "hand.h"
#include "stack.h"
#include "json.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "player_comm.h"
#include "chat_message.hpp"
#include "asio.hpp"

class Player_Game : public player_comm
{
   //Hand _hand;
  public:
   Player_Game();
   void getPlayerUUID();
   void getPlayerAnte(int anteAmount);

  private:
   Stack _stack;
   std::string _name;
   boost::uuids::uuid _uuid;
   //friend class Dealer;
};

#endif
