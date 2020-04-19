//
//
// class for the cards at hand
//
//

#include "hand.h"

bool comp_suit(const Card & a, const Card & b)
{
	return (a.suit() < b.suit());
}
bool comp_value(const Card & a, const Card & b)
{
	return (a.num() < b.num());
}

Hand::Hand ()
{
}

Hand::~Hand()
{}

void Hand::modify_hand (/*std::vector <Card> mod_hand*/)
{
 // this -> _hand = mod_hand;
}

int Hand::calc_value ()
{
  int value = -1;

  std::sort(_hand.begin(), _hand.end(), comp_suit);  //sort by suit
  if(_hand[0].suit() == _hand[4].suit()) //if first card is same suit as last, we have a flush
  {
    value = 5;  //flush
    for(int i = 0; i < 3; i++) //check for straight flush
    {
      if( _hand[i].num() == (_hand[i+1].num() - 1) )
      {
        continue;
      }
      else if(_hand[i].num() == 13 && _hand[i+1].num() == 2)
      {
        continue;
      }
      else
      {
        goto next; //not straight flush, just regular flush
      }
      if( _hand[4].num() == 13) //if last card of straight flush is ace, we have a royal flush
      {
        value = 1;  //Royal Flush
	return value;
      }
      else //otherwise its a regular straight flush
      {
        value = 2;  //Straight Flush
	return value;
      }
    }
  }
  
  next:std::sort(_hand.begin(), _hand.end(), comp_value); //sort by card value
  if( (_hand[0].num() == _hand[1].num() && _hand[1].num() == _hand[2].num() && _hand[2].num() == _hand[3].num() ) ||
      (_hand[1].num() == _hand[2].num() && _hand[2].num() == _hand[3].num() && _hand[3].num() == _hand[4].num() )   )
      // Check for Four of a Kind, either xyyyy or xxxxy
  {
    value = 3; // Four of a Kind
    return value;
  }
  
  if( (_hand[0].num() == _hand[1].num() && _hand[1].num() == _hand[2].num() && _hand[3].num() == _hand[4].num() ) ||
      (_hand[0].num() == _hand[1].num() && _hand[2].num() == _hand[3].num() && _hand[3].num() == _hand[4].num() )   )
      // Check for full house, either xxyyy or xxxyy
  {
    value = 4; // Full House
    return value;
  }
  
  if( ( _hand[0].num() == _hand[1].num() && _hand[1].num() == _hand[2].num() ) ||
      ( _hand[1].num() == _hand[2].num() && _hand[2].num() == _hand[3].num() ) ||
      ( _hand[2].num() == _hand[3].num() && _hand[3].num() == _hand[4].num() )   )
      // Check for Three of a Kind, xxxyx, yxxxz, or yzxxx
  {
    value = 7; // Three of a Kind
    return value;
  }

  if( ( _hand[0].num() == _hand[1].num() && _hand[2].num() == _hand[3].num() ) ||
      ( _hand[0].num() == _hand[1].num() && _hand[3].num() == _hand[4].num() ) ||
      ( _hand[1].num() == _hand[2].num() && _hand[3].num() == _hand[4].num() )   )
      // Check for Two Pairs, xxyyz, xxyzz, or xyyzz
  {
    value = 8; // Two Pairs
    return value;
  }

  if( _hand[0].num() == _hand[1].num()  || _hand[1].num() == _hand[2].num() || _hand[2].num() == _hand[3].num() ||
      _hand[3].num() == _hand[4].num() )
      // Check for One Pair, xxyza, xyyza, xyzza, or xyzaa
  {
    value = 9; // One Pair
    return value;
  }

  value = 10;  // Nothing, Zilch, Nada, better stay in school
  return value;

}
