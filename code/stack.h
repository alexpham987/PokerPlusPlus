#ifndef STACK_H
#define STACK_H

#include <vector>
#include <assert.h>
#include "chip.h"

//defines used to make it more clear with which color stack we are dealing with and defines the intial total amount of chips in dollars
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
    int _total; //attribute that represents the current total amount of chips in dollars
    std::vector<Chip> _stack_green; //attribute that represents the stack of green chips
    std::vector<Chip> _stack_red; //attribute that represents the stack of red chips
    std::vector<Chip> _stack_blue; //attribute that represents the stack of blue chips
};


#endif
