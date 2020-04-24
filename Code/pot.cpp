#include "pot.h"

Pot::Pot(int initialPotValue) : potValue{initialPotValue}
{}

int Pot::getCurrentPot()
{
  return potValue;
}

void Pot::addToPot(int amountToAdd)
{
  potValue += amountToAdd;
}
