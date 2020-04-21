#include "stack.h"

Stack::Stack() //:_total{initial_total} 25, 5, 2
{
  for(int i = 0; i < 3; i++)
  {
    if(i == 0)
    {
      for(int j = 0; j < 25; j++)
      {
        _stack[i].push_back(RED);
        _total += 1;
      }
    }
    else if(i == 1)
    {
      for(int j = 0; j < 5; j++)
      {
        _stack[i].push_back(GREEN);
        _total += 5;
      }
    }
    else
    {
      for(int j = 0; j < 2; j++)
      {
        _stack[i].push_back(BLUE);
        _total += 25;
      }
    }
  }
}

void Stack::add_chips(int red, int green, int blue)
{
  for(int i = 0; i < 3; i++)
  {
    if(i == 0)
    {
      for(int j = 0; j < red; j++)
      {
        _stack[i].push_back(RED);
        _total += 1;
      }
    }
    else if(i == 1)
    {
      for(int j = 0; j < green; j++)
      {
        _stack[i].push_back(GREEN);
        _total += 5;
      }
    }
    else
    {
      for(int j = 0; j < blue; j++)
      {
        _stack[i].push_back(BLUE);
        _total += 25;
      }
    }
  }
}

void Stack::remove_chips(int red, int green, int blue)
{
  for(int i = 0; i < 3; i++)
  {
    if(i == 0)
    {
      for(int j = 0; j < red; j++)
      {
        _stack[i].pop_back();
        _total -= 1;
      }
    }
    else if(i == 1)
    {
      for(int j = 0; j < green; j++)
      {
        _stack[i].pop_back();
        _total -= 1;
      }
    }
    else
    {
      for(int j = 0; j < blue; j++)
      {
        _stack[i].pop_back();
        _total -= 1;
      }
    }
  }
}

int Stack::get_total()
{
  return _total;
}

/*void Stack::add_chips(int green, int red, int blue)
{
  if(green > 0)
  {
    _stack[0] = _stack[0] + green;
  }
  if(red > 0)
  {
    _stack[1] = _stack[1] + red;
  }
  if(blue > 0)
  {
    _stack[1] = _stack[1] + red;
  }

  _total = _total + (green*green_multiplier);

  _total = _total + (red*red_multiplier);

  _total = _total + (blue*blue_multiplier);
}

void Stack::remove_chips(int green, int red, int blue)
{
  if(green > 0)
  {
    _stack[0] = _stack[0] - green;
  }
  if(red > 0)
  {
    _stack[1] = _stack[1] - red;
  }
  if(blue > 0)
  {
    _stack[1] = _stack[1] - red;
  }
  _total = _total - (green*green_multiplier);

  _total = _total - (red*red_multiplier);

  _total = _total - (blue*blue_multiplier);
}

int Stack::get_total()
{
  return _total;
}*/
