#include "round.h"

Round::Round(int roundNumber, std::string roundType) : _roundNumber{roundNumber}, : _roundType{roundType}
{}
int Round::getRoundNumber()
{
  return _roundNumber;
}
void Round::changeRoundNumber(int newRoundNumber)
{
  _roundNumber = newRoundNumber;
}
std::string Round::getRoundType()
{
  return roundType;
}
void Round::changeRoundType(std::string newRoundType)
{
  roundType = newRoundType;
}
