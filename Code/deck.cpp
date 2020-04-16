#include "deck.h"

Deck::Deck()
{
	for(int i = 1; i <= 13; i++) {
		_deck.push_back(Card(i,C));
		_deck.push_back(Card(i,D));
		_deck.push_back(Card(i,H));
		_deck.push_back(Card(i,S));
	}
	
	if(_deck.size() != 52)
		throw std::runtime_error("Deck : Out of Range!");

}

Card Deck::deal() {
	Card c = _deck.back();
	_deck.pop_back();
	return c;
		
}

void Deck::shuffle() {
	std::random_shuffle(_deck.begin(), _deck.end());
}

int Deck::remaining_cards() {
	return _deck.size();
}

bool Deck::deck_empty() {
	return _deck.empty();
}

