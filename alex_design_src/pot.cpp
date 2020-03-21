#include "pot.h"

Pot::Pot(int initialPotValue) : _initialPotValue{initialPotValue}
{}
int Pot::getCurrentPot()
{
  return potValue;
}
void Pot::addToPot(int amountToAdd)
{
  potValue = potValue + amountToAdd;
}
