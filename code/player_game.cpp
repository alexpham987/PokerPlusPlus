#include "player_game.h"
#include "player_comm.h"

Player_Game::Player_Game(){

   boost::uuids::random_generator generator;

   //boost::uuids::uuid id = generator();

}

nlohmann::json Player_Game::move_j(std::string play, std::string event, int cards_requested, int total_bet) const
{
  player_comm *p = 0;
  nlohmann::json to_dealer;
  chat_message uuid;
  std::string json_str;


  to_dealer["from"] = { {"uuid",this->id} , {"name",this->_name} };
  to_dealer["event"] = play;        // "stand","hit","fold","raise","join","request_cards"
  to_dealer["cards_requested"] = this->cards_requested;    // optional, number of cards requested, 1 to 5
  to_dealer["current_bet"] = this->current_bet;
  to_dealer["total_bet"] = this->total_bet;
  //to_dealer["chat"] = std::string(chat);

  json_str = to_dealer.dump();

  uuid.body_length(std::strlen(json_str.c_str()));
  std::memcpy(uuid.body(), json_str.c_str(), uuid.body_length());
  uuid.encode_header();

  p -> write(uuid);

  return to_dealer;
}
