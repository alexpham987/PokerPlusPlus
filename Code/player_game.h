#ifndef PLAYER_GAME_H
#define PLAYER_GAME_H

#include "hand.h"
#include "stack.h"
#include "json.hpp"
#include "chat_message.hpp"
#include <vector>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

class Player_Game
{
  public:
    Player_Game();
    void setName(std::string name);
    int getChipAmount();
    std::vector<std::string> setHand(nlohmann::json cards, int cardNum);
    chat_message move_j(std::string play, int cards_requested, int current_bet);
    chat_message exchange_j(std::string play, int cards_requested, std::vector<int> cards);

  private:
    std::string _name; //attribute that represents the player's name
    boost::uuids::uuid _id; //attribute that represents the player id
    Stack _stack; //attribute that represents the stack of chips
    Hand _hand; //attribute that represents the player's hand
};

#endif
