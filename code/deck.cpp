#include "deck.h"

Deck::Deck()
{
	//Create deck of 52 cards
	
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

Card Deck::getCard() {
}
