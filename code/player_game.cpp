//
//
// class for Player
//
//
#include "player_game.h"


Player_Game::Player_Game(){

   boost::uuids::random_generator generator;

   boost::uuids::uuid id = generator();
}

nlohmann::json Player_Game::move_j() const
{}
