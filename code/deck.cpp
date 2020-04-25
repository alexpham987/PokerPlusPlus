#include "deck.h"

//constructor
Deck::Deck()
{
	//fills the _deck attribute with cards
	for(int i = 1; i <= 13; i++)
	{
		_deck.push_back(Card(i,C));
		_deck.push_back(Card(i,D));
		_deck.push_back(Card(i,H));
		_deck.push_back(Card(i,S));
	}

  //makes sure the deck has the correct number of cards
	if(_deck.size() != 52)
	{
		throw std::runtime_error("Deck : Out of Range!");
	}
}

//method that deals cards
Card Deck::deal()
{
	Card c = _deck.back();
	_deck.pop_back();
	return c;
}

//method that shuffles the deck
void Deck::shuffle()
{
	std::random_shuffle(_deck.begin(), _deck.end());
}

//method that checks the for number of cards left in the deck
int Deck::remaining_cards()
{
	return _deck.size();
}

//method that checks to see if the deck has no cards left
bool Deck::deck_empty()
{
	return _deck.empty();
}
