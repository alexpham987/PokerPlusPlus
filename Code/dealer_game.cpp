#include "dealer_game.h"

//constructor intitializes _deck with the passed in parameter
Dealer_Game::Dealer_Game(Deck deck) : _deck{deck}
{}

void Dealer_Game::startGame()
{

  nlohmann::json move;
  int index_winner = 0;
  shuffleCards();

  //****************
  //    Ante     *
  //*****************
  
  /*****************
  **    Phase 1   *
  *****************
  // deal_cards( players[i].size() );     // deal cards to all players
   
  for( int i = 0; i < room_.participants().size(); i++ )
  {
    if( /move[event] = raise/ )         // int variable in dealer, 1 if a
                                          // player raised
    {
      // players[i].turn = 3;             // check & exchange button greyed out
      // move = code to get player move
    }
    else                                  // no one has raised yet
    {
      // players[i].turn = 2;             // exchange button greyed out
      // move = code to get player move
    }
    if(/ move[event] = fold /)
    {
      // players[i].turn = -1;            // player loses
      continue;
    }
    if(/ move[event] = check /)
    {
      continue;
    }
    if(/ move[event] = raise /)         // player raises bet by current_bet
    {
      // players[i]._stack.remove_chips( move[current_bet] );
      // pot += move[current_bet];
      // players[i].turn = 0;            // turn over
    }
  }
  ********************
  *    Draw Phase    *
  ********************
 
  for( int i = 0; i < room_.participants().size(); i++ )
  {
    // players[i].turn = 4;               // only exchange button available
    // move = code to get player move
    if(/ move[cards_requested] < 1 || move[cards_requested] > 5 /)
    {
      continue;                           // if cards requested is not in range,
    }                                     // go to next player
                                          // otherwise exchange cards
    exchange_cards( /move[cards_requested]/ );
  }
  *****************
  *    Phase 2    *
  *****************
  for( int i = 0; i < room_.participants().size(); i++ )
  {
    if( /move[event] = raise/ )         // int variable in dealer, 1 if a
                                          // player raised
    {
      // players[i].turn = 3;             // check & exchange button greyed out
      // move = code to get player move
    }
    else                                  // no one has raised yet
    {
      // players[i].turn = 2;             // exchange button greyed out
      // move = code to get player move
    }
    if(/ move[event] = fold /)
    {
      // players[i].turn = -1;            // player loses
      continue;
    }
    if(/ move[event] = check /)
    {
      continue;
    }
    if(/ move[event] = raise /)         // player raises bet by current_bet
    {
       // players[i]._stack.remove_chips( move[current_bet] );
       // pot += move[current_bet];
       // players[i].turn = 0;            // turn over
    }
  }
  *******************
  *    Game Over    *
  *******************
  for( int i = 1; i < room_.participants().size(); i++ )
  {
    if( /players[i].stack.total() > players[index_winner].stack.total()/ )
    {
      index_winner = i;                   // find the player with most money
    }
  }
  // code to let players know if they won/lost
  // winner is players[ index_winner ]
  */
}

//method that adds money to the pot
void Dealer_Game::addMoney(int amount)
{
  _pot.addToPot(amount);
}

//method that shuffles the deck of cards
void Dealer_Game::shuffleCards()
{
  _deck.shuffle();
  _deck.shuffle();
  _deck.shuffle();
}

//method that deals the cards to the player
chat_message Dealer_Game::dealCards(int num)
{
  nlohmann::json to_player;
  chat_message cards;
  std::string json_str;
  to_player["event"] = "Deal";

  for(int i = 1; i <= num; i++)
  {
    std::string in = std::to_string(i);
    Card c = _deck.deal();
    to_player[in] = c.card_to_string();
  }

  to_player["cards_requested"] = num;

  json_str = to_player.dump();

  cards.body_length(std::strlen(json_str.c_str()));
  std::memcpy(cards.body(), json_str.c_str(), cards.body_length());
  cards.encode_header();

  return cards;
}

//method that determines whether a player won or lost
bool Dealer_Game::gameResult()
{
  return false;
}

//method that exchanges the requested amount of cards in a player's hand
chat_message Dealer_Game::exchangeCards(int amountOfCards)
{
  nlohmann::json to_player;
  chat_message cards;
  std::string json_str;

  for(int i = 0; i < amountOfCards; i++)
  {
    std::string in = std::to_string(i);
    Card c = _deck.deal();
    to_player[in] = c.card_to_string();
  }

  json_str = to_player.dump();

  cards.body_length(std::strlen(json_str.c_str()));
  std::memcpy(cards.body(), json_str.c_str(), cards.body_length());
  cards.encode_header();

  return cards;
}

void Dealer_Game::setDeck(Deck deck)
{
  _deck = deck;
}

void Dealer_Game::revealHand()
{}
