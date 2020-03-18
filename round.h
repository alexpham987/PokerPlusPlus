#ifndef ROUND_H
#define ROUND_H

class Round
{
  public:
    Round(int roundNumber);
    int getRoundNumber();
    void changeRoundNumber(int newRoundNumber);

  private:
    int _roundNumber;
};

#endif
