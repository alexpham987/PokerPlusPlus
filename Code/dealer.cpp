#include "dealer.h"

#define PLAYERS 2

using asio::ip::tcp;

static int turn_index;
static int game;
static int phase;
static Deck _deck;
static Dealer_Game _dg{_deck};
static int cardsDealt;
static int value_winner;
static std::string id_winner;
static int high_card;


/*************************/
/*    Class chat_room    */
/*************************/

//adds player to game
void chat_room::join(chat_participant_ptr participant, std::string/*boost::uuids::uuid*/ id)
{
  //std::cout << "participant joined" << std::endl;
  participants_.insert(participant);
  participants_it = participants_.begin();
  std::cout << "participant added, total: " << participants().size() << std::endl;
  id_players[participant] = id; 
  if( participants().size() >= PLAYERS )
  {
    game = 1;
    turn_index = 0;
    phase = 0; //ante
    cardsDealt = 0;
    value_winner = 20;
    high_card = -1;
  }
  else
  {
    game = 0;
  }
  //std::cout << "participant id made" << std::endl;
}

//deletes player from game
void chat_room::leave(chat_participant_ptr participant)
{
  participants_.erase(participant);
  
}

//deliver message to players
void chat_room::deliver(const chat_message& msg)
{
  recent_msgs_.push_back(msg);
  while (recent_msgs_.size() > max_recent_msgs)
    recent_msgs_.pop_front();

  for (auto participant: participants_)
    participant->deliver(msg);
}

//get all current players
std::set<chat_participant_ptr> chat_room::participants() 
{
  return participants_;
}

/***************************/
/*    Class chat_session   */
/***************************/

chat_session::chat_session(tcp::socket socket, chat_room& room)
  : socket_(std::move(socket)),
    room_(room)
{
  Deck _deck;
  _deck.shuffle();
  _dg.setDeck( _deck );
  joined = 0;
}

//starts game session
void chat_session::start(Dealer_comm *_dc)
{
  boost::uuids::random_generator generator;
  boost::uuids::uuid id_uuid = generator(); 
  std::string id = boost::uuids::to_string(id_uuid);
  room_.join(shared_from_this(), id);
  room_.deliver(set_id(id)); 
  //std::cout << "set_id() ran" << std::endl;
  do_read_header();
  //std::cout << "header read" << std::endl;
  /*if(room_.participants().size() > 1 && joined > 1)
  {
    printf("\ncheck 0\n");
    startGame(_dc);
  }*/
}

//delivers chat message
void chat_session::deliver(const chat_message& msg)
{
  bool write_in_progress = !write_msgs_.empty();
  write_msgs_.push_back(msg);
  if (!write_in_progress)
  {
    do_write();
  }
}

//reads chat message header
void chat_session::do_read_header()
{
  auto self(shared_from_this());
  asio::async_read(socket_,
      asio::buffer(read_msg_.data(), chat_message::header_length),
      [this, self](std::error_code ec, std::size_t /*length*/)
      {
        if (!ec && read_msg_.decode_header())
        {
          for(unsigned int i = 0; i < chat_message::max_body_length; i++) 
          {
            read_msg_.body()[i] = '\0';
          }
          do_read_body();
        }
        else
        {
          room_.leave(shared_from_this());
        }
      });
}


//reads chat message body
void chat_session::do_read_body()
{
  //std::cout << "reading message" << std::endl;
  auto self(shared_from_this());
  asio::async_read(socket_,
      asio::buffer(read_msg_.body(), read_msg_.body_length()),
      [this, self](std::error_code ec, std::size_t /*length*/)
      {
        if (!ec)
        {
          nlohmann::json info = nlohmann::json::parse(read_msg_.body());
          nlohmann::json to_player;
          chat_message msg;
          /*if(game == 1 && room_.participants().size() == 1)
          {
            std::cout << "game over" << std::endl;
          }
	  if(info["event"] == "join")
	  {
            std::cout << "joined?, game = " << game << std::endl;
            //std::this_thread::sleep_for(std::chrono::seconds(5));
            
            set_id(room_.id_players[*(room_.participants().end())]);
            //joined++;
            if(game == 1)
            {
              room_.deliver( turn( 1, room_.id_players[*(room_.participants_it)] ) );
              //to_player["event"] = "message";
              //to_player["player_id"] = room_.id_players[*(room_.participants_it)];
              //to_player["turn"] = 1;
            }
	  }
          else if( info["event"] == "fold" && game == 1 && info["player_id"] == room_.id_players[*(room_.participants_it)] )
	  {
	    std::cout << "folded" << std::endl;
            room_.deliver(read_msg_);
            room_.participants_it++;
            room_.deliver( turn( 1, room_.id_players[*(room_.participants_it)] ) );
	  }
	  else if( info["event"] == "ante" && game == 1 && info["player_id"] == room_.id_players[*(room_.participants_it)] )
	  {
	    _dg.addMoney(1);
	    std::cout << "chip added" << std::endl;
            room_.deliver(read_msg_);
            room_.participants_it++;
            room_.deliver( turn( 1, room_.id_players[*(room_.participants_it)] ) );
	  }
	  else if( (info["event"] == "bet" || info["event"] == "call" || info["event"] == "raise") && game == 1 && (info["player_id"] == room_.id_players[*(room_.participants_it)] ) )
	  {
	    int amount = info["current_bet"];
	    _dg.addMoney(amount);
	    std::cout << "chip added" << std::endl;
            room_.deliver(read_msg_);
            room_.participants_it++;
            room_.deliver( turn( 1, room_.id_players[*(room_.participants_it)] ) );
	  }
	  else if( info["event"] == "request_cards" && game == 1 && info["player_id"] == room_.id_players[*(room_.participants_it)] )
	  {
	    std::cout << "getting cards" << std::endl;
	    int num = info["cards_requested"];
	    chat_message cards = _dg.dealCards(num);
	    room_.deliver(cards);
            room_.participants_it++;
            room_.deliver( turn( 1, room_.id_players[*(room_.participants_it)] ) );
	  }
	  else 
          {
            room_.deliver(read_msg_);
	  }*/
          do_round(info);
          do_read_header();
        }
        else
        {
          room_.leave(shared_from_this());
        }
      });
}


//writes chat message
void chat_session::do_write()
{
  auto self(shared_from_this());
  asio::async_write(socket_,
      asio::buffer(write_msgs_.front().data(),
        write_msgs_.front().length()),
      [this, self](std::error_code ec, std::size_t /*length*/)
      {
        if (!ec)
        {
          write_msgs_.pop_front();
          if (!write_msgs_.empty())
          {
            do_write();
          }
        }
        else
        {
          room_.leave(shared_from_this());
        }
      });
}

/**************************/
/*    Class Dealer_comm   */
/**************************/

Dealer_comm::Dealer_comm(asio::io_context& io_context,
    const tcp::endpoint& endpoint)
  : acceptor_(io_context, endpoint)
{
  do_accept();
  //startGame();
}

void Dealer_comm::startGame() 
{
  //std::this_thread::sleep_for(std::chrono::seconds(20));
}

void Dealer_comm::do_accept()
{
  acceptor_.async_accept(
      [this](std::error_code ec, tcp::socket socket)
      {
        if (!ec)
        {
          std::make_shared<chat_session>(std::move(socket), room_)->start(this);
        }


        do_accept();
      });
}


//constructor intitializes _deck with the passed in parameter
Dealer_Game::Dealer_Game(Deck deck) : _deck{deck}
{}

//sends the turn int to the players, if the player has the id
//sent then its their turn
chat_message chat_session::turn(int turn, std::string/*boost::uuids::uuid*/ id_player/*, std::string name*/, std::string move)
{
  nlohmann::json to_player;
  chat_message msg;
  std::string json_str;

  to_player["turn"] = turn;
  to_player["id_player"] = id_player;
  if(turn == -1)
  {
    to_player["event"] = "game over";
  }
  else
  {
    to_player["event"] = "turn";
    to_player["move"] = move;
  }

  json_str = to_player.dump();

  msg.body_length(std::strlen(json_str.c_str()));
  std::memcpy(msg.body(), json_str.c_str(), msg.body_length());
  msg.encode_header();

  return msg;
}
/*
chat_message chat_session::turn(int turn, std::string/boost::uuids::uuid id_player/, int x)
{
  nlohmann::json to_player;
  chat_message msg;
  std::string json_str;
  int num = 5;

  to_player["turn"] = turn;
  to_player["id_player"] = id_player;
  to_player["event"] = "Deal";
  for(int i = 1; i <= num; i++)
  {
    std::string in = std::to_string(i);
    Card c = _deck.deal();
    to_player[in] = c.card_to_string();
  }

  to_player["cards_requested"] = num;

  json_str = to_player.dump();

  msg.body_length(std::strlen(json_str.c_str()));
  std::memcpy(msg.body(), json_str.c_str(), msg.body_length());
  msg.encode_header();

  return msg;
}*/

chat_message chat_session::set_id(/*boost::uuids::uuid*/std::string id_player)
{
  nlohmann::json to_player;
  chat_message msg;
  std::string json_str;

  to_player["turn"] = 0;
  to_player["id_player"] = id_player;
  to_player["event"] = "set_id";

  json_str = to_player.dump();

  msg.body_length(std::strlen(json_str.c_str()));
  std::memcpy(msg.body(), json_str.c_str(), msg.body_length());
  msg.encode_header();

  return msg;
}

void chat_session::do_round(nlohmann::json info)
{
  /****************/
  /*     Ante     */
  /****************/

  if(phase == 0)
  {
    if(game == 1 && room_.participants().size() == 1)
    {
      std::cout << "game over" << std::endl;
      return;
    }
    if(info["event"] == "join")
    {              
      set_id( room_.id_players[ *( room_.participants().end() ) ] );
      room_.name_players[ *( room_.participants().end() ) ] = info["name"];
      if(game == 1)
      {
        room_.deliver( turn( 1, room_.id_players[*(room_.participants_it)], "join" ) );
      }
      return;
    }
    else if( info["event"] == "fold" && game == 1 && info["player_id"] == room_.id_players[*(room_.participants_it)] )
    {
      std::cout << "folded" << std::endl;
      room_.deliver(read_msg_);
      if( turn_index == (PLAYERS) )
      {
        room_.participants_it = room_.participants().begin();
        turn_index = 0;
      }
      else
      {
        room_.participants_it++;
        turn_index++;
      }
      room_.deliver( turn( -1, room_.id_players[*(room_.participants_it)], "fold" ) );
      return;
    }
    else if( info["event"] == "ante" && game == 1 && info["player_id"] == room_.id_players[*(room_.participants_it)] )
    {
      _dg.addMoney(1);
      std::cout << "chip added" << std::endl;
      room_.deliver(read_msg_);
      if( turn_index == (PLAYERS - 1) )
      {
        room_.deliver( turn( 1, room_.id_players[*(room_.participants_it)], "ante" ) );
        room_.participants_it = room_.participants().begin();
        turn_index = 0;
        std::cout << "good exit, dealing to " << room_.name_players[*(room_.participants_it)] << std::endl;
        phase = 1;
      }
      else
      {
        room_.participants_it++;
        turn_index++;
        room_.deliver( turn( 1, room_.id_players[*(room_.participants_it)], "ante" ) );
        return;
      }
    }
    else if( (info["event"] == "bet" || info["event"] == "call" || info["event"] == "raise") && game == 1 && (info["player_id"] == room_.id_players[*(room_.participants_it)] ) )
    {
      int amount = info["current_bet"];
      _dg.addMoney(amount);
      std::cout << "chip added" << std::endl;
      room_.deliver(read_msg_);
      if( turn_index == (PLAYERS) )
      {
        room_.participants_it = room_.participants().begin();
        turn_index = 0;
      }
      else
      {
        room_.participants_it++;
        turn_index++;
      }
      std::string event = info["event"];
      room_.deliver( turn( 1, room_.id_players[*(room_.participants_it)], event ) );
      return;
    }
    else 
    {	
      room_.deliver(read_msg_);
    }
  }
  std::cout << "phase: " << phase << " game: " << game << std::endl;

  /*****************/
  /*    Phase 1    */
  /*****************/

//deal cards

  if((phase == 1 && game == 1) || info["event"] == "dealt")
  {
    if( turn_index == 0 && info["event"] == "dealt" )
    {
      room_.participants_it = room_.participants().begin();
      turn_index = 0;
      game = 0;
      std::cout << "done dealing" << std::endl; 
      room_.deliver( turn( 2, room_.id_players[*(room_.participants_it)], "deal" ) );
      return;
    }
    //std::cout << "player to be dealt: " << room_.name_players[*(room_.participants_it)] << std::endl;
    chat_message cards = _dg.dealCards(5, room_.id_players[*(room_.participants_it)] );
    room_.deliver(cards);
    room_.deliver(read_msg_);
    if( turn_index == (PLAYERS-1) )
    {
      room_.participants_it = room_.participants().begin();
      turn_index = 0;
      game = 0;
      std::cout << "one more dealing" << std::endl; 
    }
    else
    {
      turn_index++;
      room_.participants_it = room_.participants().begin();
      std::advance(room_.participants_it, turn_index);
      return;
    }    
  }

//first bet with cards 

  else if(phase == 1 && game == 0)
  {
    std::cout << "event = " << info["event"] << std::endl;
    if( (info["event"] == "check") || (info["event"] == "fold") || (info["event"] == "bet") )
    {
      room_.deliver(read_msg_);
      std::string event = info["event"]; 

      if( turn_index == (PLAYERS-1) )
      {
        room_.participants_it = room_.participants().begin();
        turn_index = 0;
        game = 2;
        std::cout << "done" << std::endl;
        room_.deliver( turn( 3, room_.id_players[*(room_.participants_it)], event ) );
      }
      else
      {
        turn_index++;
        room_.participants_it = room_.participants().begin();
        std::advance(room_.participants_it, turn_index);
        room_.deliver( turn( 2, room_.id_players[*(room_.participants_it)], event ) );
        return;
      }
    }
  }

//chance to exchange cards

  else if(phase == 1 && game == 2)
  {
    std::cout << "event = " << info["event"] << std::endl;
    if( (info["event"] == "stand") || (info["event"] == "fold") || (info["event"] == "exchange") )
    {
      room_.deliver(read_msg_);
      std::string event = info["event"]; 
      if( turn_index == (PLAYERS-1) )
      {
        room_.participants_it = room_.participants().begin();
        turn_index = 0;
        game = 3;
        std::cout << "done phase 2" << std::endl; 
        room_.deliver( turn( 4, room_.id_players[*(room_.participants_it)], event ) );
      }
      else
      {
        turn_index++;
        room_.participants_it = room_.participants().begin();
        std::advance(room_.participants_it, turn_index);
        room_.deliver( turn( 3, room_.id_players[*(room_.participants_it)], event ) );
        return;
      }
    }
  }

  // last bet

  else if(phase == 1 && game == 3)
  {
    std::cout << "event = " << info["event"] << std::endl;
    if( (info["event"] == "check") || (info["event"] == "fold") || (info["event"] == "bet") )
    {
      room_.deliver(read_msg_);
      std::string event = info["event"]; 
      if( turn_index == (PLAYERS-1) )
      {
        room_.participants_it = room_.participants().begin();
        turn_index = 0;
        game = -1;
        phase = 3;  //players in game 
        room_.deliver( turn( -1, room_.id_players[*(room_.participants_it)], event ) );
      }
      else
      {
        turn_index++;
        room_.participants_it = room_.participants().begin();
        std::advance(room_.participants_it, turn_index);
        room_.deliver( turn( 2, room_.id_players[*(room_.participants_it)], event ) );
        return;
      }
    }
  }

  /*******************/
  /*    Game Over    */
  /*******************/

  if(game == -1)
  {
    if(phase == 1)
    {
      std::cout << "game overrrrrrr" << std::endl;
      for( std::map<std::string, int>::iterator it = room_.hand_values.begin(); it != room_.hand_values.end(); ++it )
      {
        std::cout << "inside loop" << std::endl;
        if(it->second <= value_winner)
        {
          if(it->second == value_winner)
          {
            std::cout << "we got a tie" << std::endl;
            //need more code to find winner
          }
          std::cout << "updated winner" << std::endl;
          value_winner = it->second;
          id_winner = it->first;
        }
      }
      std::cout << "announcing winner of the big dick club" << std::endl;
      room_.deliver( winner( id_winner, value_winner ) );
    }
    else//if(info["cards requested"] == -1)
    {
      std::cout << phase << std::endl;
      room_.hand_values[ info["player_id"].get<std::string>() ] = info.value("hand value", 0);
      phase--;
      std::cout << "hand value stored" << std::endl;
      room_.participants_it = room_.participants().begin();
      std::advance(room_.participants_it, turn_index);
      room_.deliver( turn( -1, room_.id_players[*(room_.participants_it)], "game over" ) );
    }

  }


}


chat_message chat_session::winner( std::string id, int value )
{
  nlohmann::json to_player;
  chat_message msg;
  std::string json_str;

  to_player["turn"] = 10;
  to_player["id_player"] = id;
  to_player["event"] = "winner";

  json_str = to_player.dump();

  msg.body_length(std::strlen(json_str.c_str()));
  std::memcpy(msg.body(), json_str.c_str(), msg.body_length());
  msg.encode_header();

  return msg;
}

//gameplay skeleton, not used, for reference
void chat_session::startGame(Dealer_comm *_dc)
{
  std::this_thread::sleep_for(std::chrono::seconds(10));
  //while(room_.participants().size() < 3);
  std::set <chat_participant_ptr> players = room_.participants();
  //std::cout << "check 1" << std::endl;
  //nlohmann::json move;
  //int index_winner = 0;
  //shuffleCards();

  /****************/
  /*     Ante     */
  /****************/

  //for( int i = 0; i < room_.participants().size(); i++ )
// while(1)
 //{
  for (auto participant: room_.participants())
  {
    //std::cout << "check 2" << std::endl;
    // players[i].turn = 1;               // array with all the players
    std::this_thread::sleep_for(std::chrono::seconds(2));
    //room_.deliver( turn( 1, room_.id_players[participant] ) );
    //room_.deliver(read_msg_);
    std::cout << "message sent" << std::endl;
    //_dc->do_accept();
    //do_read_header();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    //do_read_header();

    // move = code to get player move

    //if(/ move = fold /)
    //{
      
      // players[i].turn = -1;            // player loses
      //continue;
    //}
    // players[i]._stack.remove_chips(1); // ante
    // pot += 1;                          // ante
    // players[i].turn = 0;               // turn over
  }  
//}
  /*****************/
  /*    Phase 1    */
  /*****************/

  // deal_cards( players[i].size() );     // deal cards to all players
/*   for (auto participant: room_.participants())
  {
    chat_message cards = _dg.dealCards(5);
    room_.deliver(cards);
  }*/
/*   
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
*/
  // code to let players know if they won/lost
  // winner is players[ index_winner ]
}

//method that adds money to the pot

void Dealer_Game::setDeck(Deck _deck)
{
  this->_deck = _deck;
}

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
chat_message Dealer_Game::dealCards(int num, std::string id )
{
  nlohmann::json to_player;
  chat_message cards;
  std::string json_str;
  to_player["event"] = "Deal";
  to_player["id_player"] = id;

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

void Dealer_Game::revealHand()
{}

//main function for the dealer's side of the game
int main(int argc, char* argv[])
{
  try
  {
    if (argc < 2)
    {
      std::cerr << "Usage: chat_server <port> [<port> ...]\n";
      return 1;
    }

    asio::io_context io_context;

    std::list<Dealer_comm> dealers;
    for (int i = 1; i < argc; ++i)
    {
      tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[i]));
      dealers.emplace_back(io_context, endpoint);
    }
    //std::cout << "b4 context run" << std::endl;
    io_context.run();
    //std::this_thread::sleep_for(std::chrono::seconds(2));
    //std::cout << "after context run" << std::endl;
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }
  std::cout << "end" << std::endl;
  return 0;
}
