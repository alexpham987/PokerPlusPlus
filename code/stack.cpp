#include "stack.h"

//constructor initializes _total to INITIAL_TOTAL
Stack::Stack() :_total{INITIAL_TOTAL}
{
  _total = 0;
//for loop to initialize total chips to 100 dollars worth (25 red chips, 5 green chips, 2 blue chips)
  for(int i = 0; i < 25; i++)
  {
    _stack_green.push_back(Chip(RED));
    _total += RED_MULTIPLIER;
    if(i<5)
    {
      _stack_red.push_back(Chip(GREEN));
      _total += GREEN_MULTIPLIER;
    }
    if(i<2)
    {
      _stack_blue.push_back(Chip(BLUE));
      _total += BLUE_MULTIPLIER;
    }
  }

//if the initialized total is greater than what was initialized above, more chips need to be added below
  if(INITIAL_TOTAL > _total)
  {
    int initial = INITIAL_TOTAL;
    int temp = initial / BLUE_MULTIPLIER;
    if(temp > 0)
    {
      for(int i = 0; i < temp; i++)
      {
        _stack_blue.push_back(Chip(BLUE));
        _total += BLUE_MULTIPLIER;
        initial -= BLUE_MULTIPLIER;
      }
    }
    temp = initial / GREEN_MULTIPLIER;
    if(temp > 0)
    {
      for(int i = 0; i < temp; i++)
      {
        _stack_blue.push_back(Chip(RED));
        _total += GREEN_MULTIPLIER;
        initial -= GREEN_MULTIPLIER;
      }
    }
    temp = initial / RED_MULTIPLIER;  //for clarity, don't really need / RED_MULTIPLIER
    if(temp > 0)
    {
      for(int i = 0; i < temp; i++)
      {
        _stack_blue.push_back(Chip(RED));
        _total += RED_MULTIPLIER;
        initial -= RED_MULTIPLIER;
      }
    }
    assert(initial == 0);
  }
}

//method used to add chips to the vector that is requested
void Stack::add_chips(int green, int red, int blue)
{
  for(int i = 0; i < green; i++)
  {
    _stack_green.push_back(Chip(GREEN));
    _total += GREEN_MULTIPLIER;
  }
  for(int i = 0; i < red; i++)
  {
    _stack_red.push_back(Chip(RED));
    _total += RED_MULTIPLIER;
  }
  for(int i = 0; i < blue; i++)
  {
    _stack_blue.push_back(Chip(BLUE));
    _total += BLUE_MULTIPLIER;
  }
}

//method used to remove chips from the vector that is requested
void Stack::remove_chips(int green, int red, int blue)
{
  for(int i = 0; i < green; i++)
  {
    _stack_green.pop_back();
    _total -= GREEN_MULTIPLIER;
  }
  for(int i = 0; i < red; i++)
  {
    _stack_red.pop_back();
    _total -= RED_MULTIPLIER;
  }
  for(int i = 0; i < blue; i++)
  {
    _stack_blue.pop_back();
    _total -= BLUE_MULTIPLIER;
  }
}

//method used to get the _total attribute
int Stack::get_total()
{
  return _total;
}
