#ifndef STACK_H
#define STACK_H

#include <vector>
#include "chip.h"

/*#define initial_total 100
#define red_multiplier 1
#define green_multiplier 5
#define blue_multiplier 25*/

class Stack
{
  friend class Dealer;
  friend class Player_Game;

  public:
    Stack();
    void add_chips(int red, int green, int blue);
    void remove_chips(int red, int green, int blue);
    int get_total();

  private:
    int _total;
    //std::vector<Chip> _stack;
    std::vector<std::vector<Chip>> _stack;
};


#endif
