//
//
// class for Player
//
//
#include "player_game.h"
#include "player_comm.h"

Player_Game::Player_Game(){

  
   boost::uuids::random_generator generator;

   boost::uuids::uuid id = generator();


}

nlohmann::json Player_Game::move_j() const
{
  nlohmann::json to_dealer;
  to_dealer["from"] = { {"uuid",this->id} , {"name",this->_name} };
  to_dealer["event"] = this->play;        // "stand","hit","fold","raise","join","request_cards"
  to_dealer["cards_requested"] = this->cards_requested;    // optional, number of cards requested, 1 to 5
  to_dealer["current_bet"] = this->current_bet;
  to_dealer["total_bet"] = this->total_bet;
  //to_dealer["chat"] = std::string(chat);

  return to_dealer
}
