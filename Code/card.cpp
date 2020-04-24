#include "card.h"

Card::Card(int num, Suit suit) : _num{num}, _suit{suit} 
{
	if(num < MIN_NUM || num > MAX_NUM)
		throw std::runtime_error("Number Out of Range!");
	
	if(suit < 0 || suit > MAX_SUIT)
		throw std::runtime_error("Suit Out of Range!");
}

	
int Card::num() const {return _num;}

Suit Card::suit() const {return _suit;}

int Card::compareCard(Card C) {
	int num;
	if(_num == 11 || _num == 12 || _num == 13)
		num = 11;
	else
		num = _num;

	if(num < C.num()) return -1;
	else if(num > C.num()) return 1;
	else return 0;
		
	return 0;
}

std::string Card::card_to_string() {
	std::stringstream ss;

	ss << std::to_string(_num);
	ss << " ";

	std::map<Suit, std::string> suit_string = 
	{
		{Suit::C, "C"},
		{Suit::D, "D"},
		{Suit::H, "H"},
		{Suit::S, "S"}
	};
	ss << suit_string[_suit];

	return ss.str();
}

std::string Card::card_to_filename() {
	std::stringstream ss;
	ss << "Cards/";
	if(_num == 13)
		ss << "K";
	else if(_num == 12)
		ss << "Q";
	else if(_num == 11) 
		ss << "J";
	else
		ss << _num;

	std::map<Suit, std::string> suit_string = 
	{
		{Suit::C, "C"},
		{Suit::D, "D"},
		{Suit::H, "H"},
		{Suit::S, "S"}
	};
	ss << suit_string[_suit];
	ss << ".jpg";

	return ss.str();
}
	


