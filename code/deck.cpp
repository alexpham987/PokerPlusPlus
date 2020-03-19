#include "deck.h"

Deck::Deck()
{
	for(int i = 0; i < MAX_SUIT; i++) {
		for(int j = 0; j < 13; j++) {
			_deck.push_back(Card(j,static_cast<Suit>(i)));
		}
	}
	
	if(_deck.size() != 52)
		throw std::runtime_error("Deck : Out of Range!");

}

Card Deck::deal() {
	return Card(0,S);
	
}

int Deck::remaining_cards() {
	return 0;
}

bool Deck::deck_empty() {
	return 0;
}