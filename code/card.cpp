#include "card.h"

Card::Card(int num, Suit suit) : _num{num}, _suit{suit} 
{
	if(num < MIN_NUM || num > MAX_NUM)
		throw std::runtime_error("Number : Out of Range!");
	
	if(suit < 0 || suit > MAX_SUIT)
		throw std::runtime_error("Suit : Out of Range!");
}
	
int Card::num() const {return _num;}

Suit Card::suit() const {return _suit;}

int Card::compareCard() {
	return 0;
}

std::string Card::card_to_string() {
	return " ";
}


