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
    int _roundNumber;
    std::string _roundType;
};

#endif
