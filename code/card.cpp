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
	std::stringstream ss;

	switch (_num) {
		case 1: ss << "Ace of ";
			break;
		case 2: ss << "Two of ";
			break;
		case 3: ss << "Three of ";
			break;
		case 4: ss << "Four of ";
			break;
		case 5: ss << "Five of ";
			break;
		case 6: ss << "Five of ";
			break;
		case 7: ss << "Seven of ";
			break;
		case 8: ss << "Eight of ";
			break;
		case 9: ss << "Nine of ";
			break;
		case 10: ss << "Ten of ";
			break;
		case 11: ss << "Jack of ";
			break;
		case 12: ss << "Queen of ";
			break;
		case 13: ss << "King of";
			break;
	}
	return " ";
}


