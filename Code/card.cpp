#include "card.h"

//constructor initializes the attributes to the passed in parameters
Card::Card(int num, Suit suit) : _num{num}, _suit{suit}
{
  //error checking for each card created
  if(num < MIN_NUM || num > MAX_NUM)
    throw std::runtime_error("Number Out of Range!");

  if(suit < 0 || suit > MAX_SUIT)
    throw std::runtime_error("Suit Out of Range!");
}

//method that returns the value of the _num attribute
int Card::num() const
{
  return _num;
}

//method that returns the value of the _suit attribute
Suit Card::suit() const
{
  return _suit;
}

//method that compares the value of cards to determine which is greater
int Card::compareCard(Card C)
{
  int num;
  if(_num == 11 || _num == 12 || _num == 13)
    num = 11;
  else
    num = _num;

  if(num < C.num())
    return -1;
  else if(num > C.num())
    return 1;
  else
    return 0;

	return 0;
}

//method that determines what the card is based on _num and _suit and forms a string representation of it
std::string Card::card_to_string()
{
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

//method that converts the card to the correct filename
std::string Card::card_to_filename()
{
  std::stringstream ss;
  ss << "Cards/";
  if(_num == 13)
    ss << "K";
  else if(_num == 12)
    ss << "Q";
  else if(_num == 11)
    ss << "J";
  else if(_num == 1)
    ss << "A";
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
