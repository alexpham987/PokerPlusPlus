#include "dealer_game.h"

Dealer_Game::Dealer_Game(bool playerResult) : _playerResult{playerResult}
{}

void Dealer_Game::shuffleCards()
{
	deck.shuffle();
	deck.shuffle();
	deck.shuffle();
}
void Dealer_Game::dealCards(chat_room& c)
{
	std::set<chat_participant_ptr> participants = c.getParticipants();
	for (auto participant: participants) {
		for(int i = 0; i < 5; i++) 
     		std::cout << "participant->deliverCards(deck.deal());" << std::endl;
	}

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
