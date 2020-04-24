#include "dealer_game.h"

//constructor intitializes _playerResult with the passed in parameter
Dealer_Game::Dealer_Game(bool playerResult) : _playerResult{playerResult}
{}

//void Dealer_Game::startGame()
//{

//method that shuffles the deck of cards
void Dealer_Game::shuffleCards()
{
	//does not compile
	//_deck.shuffle();
	//_deck.shuffle();
	//_deck.shuffle();
}

//method that deals the cards to the player
chat_message Dealer_Game::dealCards()
{
	nlohmann::json to_player;
	chat_message cards;
	std::string json_str;
	to_player["event"] = "Deal";

	for(int i = 0; i < 5; i++)
	{
		std::string in = std::to_string(i);
		Card c = _deck.deal();
		to_player[in] = c.card_to_string();
	}

  json_str = to_player.dump();

  cards.body_length(std::strlen(json_str.c_str()));
 	std::memcpy(cards.body(), json_str.c_str(), cards.body_length());
  cards.encode_header();

	return cards;
}

//method that determines whether a player won or lost
bool Dealer_Game::gameResult()
{
  return false;
}

//method that exchanges the requested amount of cards in a player's hand
chat_message Dealer_Game::exchangeCards(int amountOfCards)
{
	nlohmann::json to_player;
	chat_message cards;
	std::string json_str;

	for(int i = 0; i < amountOfCards; i++)
	{
		std::string in = std::to_string(i);
		Card c = _deck.deal();
		to_player[in] = c.card_to_string();
	}

  	json_str = to_player.dump();

  	cards.body_length(std::strlen(json_str.c_str()));
 	std::memcpy(cards.body(), json_str.c_str(), cards.body_length());
  	cards.encode_header();

	return cards;
}

//method that reveals the player's hand
void Dealer_Game::revealHand()
{}
