#ifndef STACK_H
#define STACK_H

#include <vector>
#include "chip.h"

#define initial_total 100
#define red_multiplier 1
#define green_multiplier 5
#define blue_multiplier 25

class Stack
{
  friend class Dealer;

  public:
    Stack();
    void add_chips(int green, int red, int blue);
    void remove_chips(int green, int red, int blue);
    int get_total();

  private:
    int _total;
    std::vector<int> _stack;
};


#endif
