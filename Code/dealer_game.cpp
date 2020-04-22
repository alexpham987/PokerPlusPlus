#include "dealer_game.h"

Dealer_Game::Dealer_Game(bool playerResult) : _playerResult{playerResult}
{}

void Dealer_Game::shuffleCards()
{
	_deck.shuffle();
	_deck.shuffle();
	_deck.shuffle();
}

chat_message Dealer_Game::dealCards()
{
	nlohmann::json to_player;
	chat_message uuid;
	std::string json_str;

	for(int i = 0; i < 5; i++) {
		std::string in = std::to_string(i);
		Card c = _deck.deal();
		to_player[in] = c.card_to_filename();
	}

  	json_str = to_player.dump();

  	uuid.body_length(std::strlen(json_str.c_str()));
 	std::memcpy(uuid.body(), json_str.c_str(), uuid.body_length());
  	uuid.encode_header();

	return uuid;

}
void Dealer_Game::dealChips()
{}
bool Dealer_Game::gameResult()
{
  return false;
}
void Dealer_Game::exchangeCards(int amountOfCards)
{}
void Dealer_Game::revealHand()
{}
