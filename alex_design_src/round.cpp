#include "round.h"

Round::Round(int roundNumber, std::string roundType) : _roundNumber{roundNumber}, _roundType{roundType}
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
  return _roundType;
}
void Round::changeRoundType(std::string newRoundType)
{
  _roundType = newRoundType;
}
