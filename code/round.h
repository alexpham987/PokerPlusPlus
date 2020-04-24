#ifndef ROUND_H
#define ROUND_H

#include <string>

class Round
{
  public:
    Round(int roundNumber, std::string roundType);
    int getRoundNumber();
    void changeRoundNumber(int newRoundNumber);
    std::string getRoundType();
    void changeRoundType(std::string newRoundType);

  private:
    int _roundNumber; //attribute that represents the current round number
    std::string _roundType; //attribute that represents the type of round
};

#endif
