//
//
// class for Player
//
//
#include "player.h"


Player::Player(){

   boost::uuids::random_generator generator;

   boost::uuids::uuid id = generator();
}

nlohmann::json Player::move_j() const
{}

