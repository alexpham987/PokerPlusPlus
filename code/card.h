#ifndef __CARD_H
#define __CARD_H

#include <string>
#include <vector>
#include <stdexcept>

enum Suit {C,D,H,S};
const std::vector<std::string> suit_to_string = {"C", "D", "H", "S"};

const int MAX_SUIT = 4;
const int MIN_NUM = 2;
const int MAX_NUM = 10;

class Card {

  public:
	Card(int num, Suit suit);
	int num () const;
	Suit suit() const;
	std::string card_to_string();
	
  private:
	int _num;
	Suit _suit;
};

#endif