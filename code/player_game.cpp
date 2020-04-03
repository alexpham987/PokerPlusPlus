#include "player_game.h"

Player_Game::Player_Game()
{
   boost::uuids::random_generator generator;
   _uuid = generator();
}
void Player_Game::getPlayerUUID()
{
  boost::uuids::to_string(_uuid);

  chat_message uuid;

  nlohmann::json to_dealer;
	to_dealer["UUID"] = _uuid;

	std::string json_str = to_dealer.dump();

	uuid.body_length(std::strlen(json_str.c_str()));
	std::memcpy(uuid.body(), json_str.c_str(), uuid.body_length());
	uuid.encode_header();

  player_comm.write(uuid);
}
/*void Player_Game::getPlayerAnte(int anteAmount)
{
  std::anteStr = std::to_string(anteAmount);
  chat_message ante;

  nlohmann::json to_dealer;
	to_dealer["ante"] = anteStr;

	std::string json_str = to_dealer.dump();

	ante.body_length(std::strlen(json_str.c_str()));
	std::memcpy(ante.body(), json_str.c_str(), ante.body_length());
	ante.encode_header();

  player_comm.write(uuid);
}*/

//nlohmann::json Player_Game::move_j() const
//{}
