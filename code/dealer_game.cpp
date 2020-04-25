#include "dealer_game.h"

//constructor intitializes _deck with the passed in parameter
Dealer_Game::Dealer_Game(Deck deck) : _deck{deck}
{}

//void Dealer_Game::startGame()
//{

//method that adds money to the pot
void Dealer_Game::addMoney(int amount)
{
  _pot.addToPot(amount);
}

//method that shuffles the deck of cards
void Dealer_Game::shuffleCards()
{
	_deck.shuffle();
	_deck.shuffle();
	_deck.shuffle();
}

//method that deals the cards to the player
chat_message Dealer_Game::dealCards(int num)
{
	nlohmann::json to_player;
	chat_message cards;
	std::string json_str;
	to_player["event"] = "Deal";

	for(int i = 1; i <= num; i++)
	{
		std::string in = std::to_string(i);
		Card c = _deck.deal();
		to_player[in] = c.card_to_string();
	}

	to_player["cards_requested"] = num;

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

void Dealer_Game::revealHand()
{}
