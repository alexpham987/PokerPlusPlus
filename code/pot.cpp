#include "pot.h"

//constructor takes an intitial pot value and sets the attribute potValue to that value
Pot::Pot(int initialPotValue) : potValue{initialPotValue}
{}

//method that returns the value of the potValue attribute
int Pot::getCurrentPot()
{
  return potValue;
}

//method that adds to the value of the potValue attribute
void Pot::addToPot(int amountToAdd)
{
  potValue = potValue + amountToAdd;
}
