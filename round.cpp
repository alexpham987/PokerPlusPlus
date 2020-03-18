#include "round.h"

Round::Round(int roundNumber) : _roundNumber{roundNumber}
{}
int Round::getRoundNumber()
{
  return _roundNumber;
}
void Round::changeRoundNumber(int newRoundNumber)
{
  _roundNumber = newRoundNumber;
}
