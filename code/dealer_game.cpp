#include "dealer_game.h"

Dealer_Game::Dealer_Game(bool playerResult, Deck deck, Stack stack) : _playerResult{playerResult}, _d{deck}, _s{stack}
{}
void Dealer_Game::shuffleCards()
{
  std::shuffle(std::begin(_d._deck), std::end(_d._deck), random);
}
void Dealer_Game::dealCards()
{}
void Dealer_Game::dealChips()
{}
bool Dealer_Game::gameResult()
{
  return false;
}
void Dealer_Game::exchangeCards(int amountOfCards)
{}
void Dealer_Game::revealHand()
{}
