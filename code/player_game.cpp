#include "player_game.h"

//constructor that intitializes the _id attribute with a random value
Player_Game::Player_Game()
{
   boost::uuids::random_generator generator;
   boost::uuids::uuid id = generator();
   _id = id;
}

//method that updates the _name attribute with the passed in parameter
void Player_Game::setName(std::string name)
{
  _name = name;
}

//Changed this function a little to make it easier to use in mainwin
//method that returns the move that a player chooses
chat_message Player_Game::move_j(std::string play, int cards_requested, int current_bet) const
{
  nlohmann::json to_dealer;
  chat_message uuid;
  std::string json_str;

  to_dealer["uuid"] = this->_id;
  to_dealer["name"] = this->_name;
  to_dealer["event"] = play;        // "stand","hit","fold","raise","join","request_cards"
  to_dealer["cards_requested"] = cards_requested;    // optional, number of cards requested, 1 to 5
  to_dealer["current_bet"] = current_bet;
  //to_dealer["total_bet"] = this->total_bet;
  //to_dealer["chat"] = std::string(chat);

  json_str = to_dealer.dump();

  uuid.body_length(std::strlen(json_str.c_str()));
  std::memcpy(uuid.body(), json_str.c_str(), uuid.body_length());
  uuid.encode_header();

  return uuid;
}

//method that exchanges the cards in a player's hand
chat_message Player_Game::exchange_j(std::string play, int cards_requested, std::vector<int> cards)
{
  _hand.modify_hand(cards);

  nlohmann::json to_dealer;
  chat_message uuid;
  std::string json_str;

  to_dealer["uuid"] = this->_id;
  to_dealer["name"] = this->_name;
  to_dealer["event"] = "request_cards";        // "stand","hit","fold","raise","join","request_cards"
  to_dealer["cards_requested"] = cards_requested;    // optional, number of cards requested, 1 to 5
  to_dealer["current_bet"] = 0;
  //to_dealer["total_bet"] = this->total_bet;
  //to_dealer["chat"] = std::string(chat);

  json_str = to_dealer.dump();

  uuid.body_length(std::strlen(json_str.c_str()));
  std::memcpy(uuid.body(), json_str.c_str(), uuid.body_length());
  uuid.encode_header();

  return uuid;
}
