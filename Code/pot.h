#ifndef POT_H
#define POT_H

class Pot
{
  public:
    Pot(int initialPotValue);
    int getCurrentPot();
    void addToPot(int amountToAdd);

  private:
    int potValue;
};

#endif
