#include "stack.h"

Stack::Stack() :_total{initial_total}
{
  _stack.push_back(1);
  _stack.push_back(1);
  _stack.push_back(1);
}

void Stack::add_chips(int green, int red, int blue)
{
  _stack[0] = _stack[0] + green;
  _total = _total + (green*green_multiplier);

  _stack[1] = _stack[1] + red;
  _total = _total + (red*red_multiplier);

  _stack[2] = _stack[2] + blue;
  _total = _total + (blue*blue_multiplier);
}

void Stack::remove_chips(int green, int red, int blue)
{
  _stack[0] = _stack[0] - green;
  _total = _total - (green*green_multiplier);

  _stack[1] = _stack[2] - red;
  _total = _total - (red*red_multiplier);

  _stack[3] = _stack[3] - blue;
  _total = _total - (blue*blue_multiplier);
}

int Stack::get_total()
{
  return _total;
}
