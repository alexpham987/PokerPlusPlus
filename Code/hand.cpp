#include "hand.h"

//method that compares the suit of two cards
bool comp_suit(const Card & a, const Card & b)
{
  return (a.suit() < b.suit());
}

//method that compares the value of two cards
bool comp_value(const Card & a, const Card & b)
{
  return (a.num() < b.num());
}

//constructor
Hand::Hand()
{
  //hand_value = calc_value();
}

//destructor
Hand::~Hand()
{}

//method that adds a card to the hand
void Hand::addCard(Card c)
{
  _hand.push_back(c);
  /*if(_hand.size() > 5)
  {
    _hand.erase(_hand.begin());
  }*/
}

//method that returns the hand itself
std::vector<Card> Hand::getHand()
{
  return _hand;
}

//method that modifies the hand
void Hand::modify_hand (std::vector<int> mod_cards)
{
  int offset = 1;
  for(auto i : mod_cards)
  {
    _hand.erase(_hand.begin() + (i-offset));
    offset++;
  }
}

//method that returns the value of the highest card
int Hand::find_high_card()
{
  std::sort(_hand.begin(), _hand.end(), comp_value); //sort by card value
  if(_hand.front().num() == 1) // ace
  {
    return 14;
  }
  return _hand.back().num();
}

//method that returns the value of the specified highest card
int Hand::find_next_highest()
{
  static int tie_round = 0;
  tie_round ++;
  int val = calc_value();
  std::sort(_hand.begin(), _hand.end(), comp_value); //sort by card value

  if( tie_round > 1 ) //second tie breaker, returns high card
  {
    goto a;
  }

  if( val == 9 || val == 7 || val == 3 ) //pair, three of a kind, four of a kind
  {
    for( int i = 0; i < 4; i++ )
    {
      if( _hand[i].num() == _hand[i+1].num() )
      {
        if( _hand[i].num() == 1 )
        {
          return 1;
        }
        return 15 - _hand[i].num();
      }
    }
  }

  if( val == 8 ) //two pairs
  {
    int high_pair = 0;
    for( int i = 0; i < 4; i++ )
    {
      if( _hand[i].num() == _hand[i+1].num() )
      {
        if( _hand[i].num() == 1 )
        {
          return 1;
        }
        if( _hand[i].num() > high_pair )
        {
          high_pair = _hand[i].num();
        }
      }
    }
    return 15 - high_pair;
  }

  if( val == 4 ) //full house
  {
    for( int i = 0; i < 3; i++ )
    {
      if( ( _hand[i].num() == _hand[i+1].num() ) && ( _hand[i+1].num() == _hand[i+2].num() ) )
      {
        if( _hand[i].num() == 1 )
        {
          return 1;
        }
        return 15 - _hand[i].num();
      }
    }
  }

  //royal flush tie unlikely, buy lottery tickets if it happnes
  //next code is for straight flush, flush, straight

a:if(_hand.front().num() == 1) // ace
  {
    return 1;
  }
  return 15 - _hand.at(4).num(); //highest card
}

//method that calculates the total value of a hand
int Hand::calc_value ()
{
  int value = 24;

  std::sort(_hand.begin(), _hand.end(), comp_suit);  //sort by suit
  if(_hand[0].suit() == _hand[4].suit()) //if first card is same suit as last, we have a flush
  {
    value = 5;  //flush
    for(int i = 0; i < 3; i++) //check for straight flush
    {
      if( _hand[i].num() == (_hand[i+1].num() - 1) )
        continue;
      else if(_hand[i].num() == 13 && _hand[i+1].num() == 2)
        continue;
      else
        goto next; //not straight flush, just regular flush

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
      (_hand[1].num() == _hand[2].num() && _hand[2].num() == _hand[3].num() && _hand[3].num() == _hand[4].num() )  )
      // Check for Four of a Kind, either xyyyy or xxxxy
  {
    value = 3; // Four of a Kind
    return value;
  }

  if( ( _hand[0].num() == (_hand[1].num()+1) ) && ( _hand[1].num() == (_hand[2].num()+1) ) && 
      ( _hand[2].num() == (_hand[3].num()+1) ) && ( _hand[3].num() == (_hand[4].num()+1) )  )
  {
    value = 6; //Straight
    return value;
  }

  if( (_hand[0].num() == _hand[1].num() && _hand[1].num() == _hand[2].num() && _hand[3].num() == _hand[4].num() ) ||
      (_hand[0].num() == _hand[1].num() && _hand[2].num() == _hand[3].num() && _hand[3].num() == _hand[4].num() )  )
      // Check for full house, either xxyyy or xxxyy
  {
    value = 4; // Full House
    return value;
  }

  if( ( _hand[0].num() == _hand[1].num() && _hand[1].num() == _hand[2].num() ) ||
      ( _hand[1].num() == _hand[2].num() && _hand[2].num() == _hand[3].num() ) ||
      ( _hand[2].num() == _hand[3].num() && _hand[3].num() == _hand[4].num() )  )
      // Check for Three of a Kind, xxxyx, yxxxz, or yzxxx
  {
    value = 7; // Three of a Kind
    return value;
  }

  if( ( _hand[0].num() == _hand[1].num() && _hand[2].num() == _hand[3].num() ) ||
      ( _hand[0].num() == _hand[1].num() && _hand[3].num() == _hand[4].num() ) ||
      ( _hand[1].num() == _hand[2].num() && _hand[3].num() == _hand[4].num() )  )
      // Check for Two Pairs, xxyyz, xxyzz, or xyyzz
  {
    value = 8; // Two Pairs
    return value;
  }

  if( _hand[0].num() == _hand[1].num() || _hand[1].num() == _hand[2].num() || _hand[2].num() == _hand[3].num() ||
      _hand[3].num() == _hand[4].num() )
      // Check for One Pair, xxyza, xyyza, xyzza, or xyzaa
  {
    value = 9; // One Pair
    return value;
  }

  value -= find_high_card();  // Nothing, Zilch, Nada, better stay in school. Value determined by highest card

  return value;
}
