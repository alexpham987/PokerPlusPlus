#ifndef STACK_H
#define STACK_H

#include <vector>
#include <assert.h>
#include "chip.h"

#define RED_MULTIPLIER 1
#define GREEN_MULTIPLIER 5
#define BLUE_MULTIPLIER 25
#define INITIAL_TOTAL 100


class Stack
{
  public:
    Stack();
    void add_chips(int green, int red, int blue);
    void remove_chips(int green, int red, int blue);
    int get_total();

  private:
    int _total;
    std::vector<Chip> _stack_green;
    std::vector<Chip> _stack_red;
    std::vector<Chip> _stack_blue;
};


#endif
