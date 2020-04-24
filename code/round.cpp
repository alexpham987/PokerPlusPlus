#include "round.h"

//constructor intitializes the attributes with the passed in parameters
Round::Round(int roundNumber, std::string roundType) : _roundNumber{roundNumber}, _roundType{roundType}
{}

//method that gets the _roundNumber attribute
int Round::getRoundNumber()
{
  return _roundNumber;
}

//method that updates the _roundNumber attribute
void Round::changeRoundNumber(int newRoundNumber)
{
  _roundNumber = newRoundNumber;
}

//method that gets the _roundType attribute
std::string Round::getRoundType()
{
  return _roundType;
}

//method that updates the _roundType attribute 
void Round::changeRoundType(std::string newRoundType)
{
  _roundType = newRoundType;
}
