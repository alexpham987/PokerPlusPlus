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
static int idx_card;
static int current_bet;


/*************************/
/*    Class chat_room    */
/*************************/

//adds player to game
void chat_room::join(chat_participant_ptr participant, std::string id)
{
  participants_.insert(participant);
  participants_it = participants_.begin();
  id_players[participant] = id; 
  if( participants().size() >= PLAYERS )
  {
    game = 1;
    turn_index = 0;
    phase = 0; //ante
    cardsDealt = 0;
    value_winner = 25;
    high_card = -1;
    idx_card = 0;
  }
  else
  {
    game = 0;
  }
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
  do_read_header();
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
chat_message chat_session::turn(int turn, std::string id_player, std::string move, int bet, int cards)
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
  else if(turn == -2)
  {
    to_player["event"] = "tie";
    to_player["index card"] = bet;
  }
  else
  {
    to_player["event"] = "turn";
    to_player["move"] = move;
  }

  to_player["cards_requested"] = cards; //optional, number of cards requested, 1 to 5
  to_player["current_bet"] = bet;

  json_str = to_player.dump();

  msg.body_length(std::strlen(json_str.c_str()));
  std::memcpy(msg.body(), json_str.c_str(), msg.body_length());
  msg.encode_header();

  return msg;
}

chat_message chat_session::set_id(std::string id_player)
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


//gameplay from dealer side
void chat_session::do_round(nlohmann::json info)
{
  /****************/
  /*     Ante     */
  /****************/
std::cout << "phase = " << phase << "   game = " << game << std::endl;
  if(phase == 0)
  {
    if(game == 1 && room_.participants().size() == 1)
    {
      std::cout << "game over" << std::endl;
      return;
    }

    std::cout << "\n\nevent and id = " << info["event"] << " ,  " << info["player_id"] << "\nmatching id:" << room_.id_players[*(room_.participants_it)] << "\n" << std::endl;

    if(info["event"] == "join")
    {              
      set_id( room_.id_players[ *( room_.participants().end() ) ] );
      room_.name_players[ *( room_.participants().end() ) ] = info["name"];
      if(game == 1)
      {
        room_.deliver( turn( 1, room_.id_players[*(room_.participants_it)], "join", 0, 0 ) );
      }
      return;
    }
    else if( info["event"] == "fold" && game == 1 && info["player_id"] == room_.id_players[*(room_.participants_it)] )
    {
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
      room_.deliver( turn( -1, room_.id_players[*(room_.participants_it)], "fold", 0, 0 ) );
      return;
    }
    else if( info["event"] == "ante" && game == 1 && info["player_id"] == room_.id_players[*(room_.participants_it)] )
    {
      _dg.addMoney(1);
      std::cout << "chip added" << std::endl;
      room_.deliver(read_msg_);
      if( turn_index == (PLAYERS - 1) )
      {
        room_.deliver( turn( 1, room_.id_players[*(room_.participants_it)], "ante", 0, 0 ) );
        room_.participants_it = room_.participants().begin();
        turn_index = 0;
        phase = 1;
      }
      else
      {
        room_.participants_it++;
        turn_index++;
        room_.deliver( turn( 1, room_.id_players[*(room_.participants_it)], "ante", 0, 0 ) );
        return;
      }
    }
    else 
    {	
      room_.deliver(read_msg_);
    }
  }

  /*****************/
  /*   Deal Cards  */
  /*****************/

  if((phase == 1 && game == 1) || info["event"] == "dealt")
  {
    if( turn_index == 0 && info["event"] == "dealt" )
    {
      room_.participants_it = room_.participants().begin();
      turn_index = 0;
      game = 0;
      std::cout << "done dealing" << std::endl; 
      room_.deliver( turn( 2, room_.id_players[*(room_.participants_it)], "deal", 0, 5 ) );
      return;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    chat_message cards = _dg.dealCards(5, room_.id_players[*(room_.participants_it)] );
    room_.deliver(cards);
    room_.deliver(read_msg_);
    if( turn_index == (PLAYERS-1) )
    {
      room_.participants_it = room_.participants().begin();
      turn_index = 0;
      game = 0;
      current_bet = 0;
    }
    else
    {
      turn_index++;
      room_.participants_it = room_.participants().begin();
      std::advance(room_.participants_it, turn_index);
      return;
    }    
  }

  /*****************************/
  /*   First Bet (with cards)  */
  /*****************************/ 

  else if(phase == 1 && game == 0)
  {
    std::cout << "event = " << info["event"] << std::endl;
    if( (info["event"] == "check") || (info["event"] == "fold") || (info["event"] == "bet") || (info["event"] == "call") || (info["event"] == "raise") )
    {

      int amount = info["current_bet"];
      _dg.addMoney(amount);
      current_bet = amount;
      std::cout << "bet chip added" << std::endl;

      room_.deliver(read_msg_);
      std::string event = info["event"]; 

      if( turn_index == (PLAYERS-1) )
      {
        room_.participants_it = room_.participants().begin();
        turn_index = 0;
        game = 2;
        room_.deliver( turn( 3, room_.id_players[*(room_.participants_it)], event, current_bet, 0 ) );
        current_bet = 0;
      }
      else
      {
        turn_index++;
        room_.participants_it = room_.participants().begin();
        std::advance(room_.participants_it, turn_index);
        room_.deliver( turn( 2, room_.id_players[*(room_.participants_it)], event, current_bet, 0 ) );
        return;
      }
    }
  }

  /*******************/
  /*  Card Exchange  */
  /*******************/

  else if(phase == 1 && game == 2)
  {
    std::cout << "event = " << info["event"] << std::endl;
    if( (info["event"] == "stand") || (info["event"] == "fold") || (info["event"] == "request_cards") )
    {
      room_.deliver(read_msg_);
      std::string event = "Deal"; 
      if(info["event"] == "request_cards")
      {
        std::string c; 
        int cards = info["cards_requested"];
        room_.participants_it = room_.participants().begin();
        std::advance(room_.participants_it, turn_index);
        chat_message msg = _dg.exchangeCards(cards, room_.id_players[*(room_.participants_it)]);
        room_.deliver( msg );
      }

      if( turn_index == (PLAYERS-1) )
      {
        room_.participants_it = room_.participants().begin();
        turn_index = 0;
        game = 3;
        room_.deliver( turn( 4, room_.id_players[*(room_.participants_it)], event, 0, 0 ) );
      }
      else
      {
        turn_index++;
        room_.participants_it = room_.participants().begin();
        std::advance(room_.participants_it, turn_index);
        room_.deliver( turn( 3, room_.id_players[*(room_.participants_it)], event, 0, 0 ) );
        return;
      }
    }
  }

  /*****************/
  /*   Final Bet   */
  /*****************/

  else if(phase == 1 && game == 3)
  {
    std::cout << "event = " << info["event"] << std::endl;
    if( (info["event"] == "check") || (info["event"] == "fold") || (info["event"] == "bet") || (info["event"] == "call") || (info["event"] == "raise") )
    {
      int amount = info["current_bet"];
      _dg.addMoney(amount);
      current_bet = amount;
      std::cout << "bet chip added" << std::endl;

      room_.deliver(read_msg_);
      std::string event = info["event"]; 
      if( turn_index == (PLAYERS-1) )
      {
        room_.participants_it = room_.participants().begin();
        turn_index = 0;
        game = -1;
        phase = PLAYERS+1;  //players in game 
        room_.deliver( turn( -1, room_.id_players[*(room_.participants_it)], event, 0, 0 ) );
        return;
      }
      else
      {
        turn_index++;
        room_.participants_it = room_.participants().begin();
        std::advance(room_.participants_it, turn_index);
        room_.deliver( turn( 2, room_.id_players[*(room_.participants_it)], event, current_bet, 0 ) );
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
done: std::cout << "game over" << std::endl;
      for( auto it: room_.hand_values )
      {
        if(it.second <= value_winner)
        {
          if(it.second == value_winner)
          {
            game = -2;
            phase = PLAYERS+1;
            std::cout << "we got a tie" << std::endl;
            turn_index = 0;
            idx_card++;
            room_.participants_it = room_.participants().begin();
            std::advance(room_.participants_it, turn_index);
            value_winner = 25;
            room_.deliver( turn( -2, room_.id_players[*(room_.participants_it)], info["event"], 0, 0 ) );
            return;
          }
          value_winner = it.second;
          id_winner = it.first;
        }
      }
      std::cout << "announcing winner" << std::endl;
      room_.deliver( winner( id_winner, value_winner ) );
    }
    else
    {
      room_.hand_values[ info["player_id"].get<std::string>() ] = info.value("hand value", 0);
      phase--;
      turn_index++;
      room_.participants_it = room_.participants().begin();
      std::advance(room_.participants_it, turn_index);
      room_.deliver( turn( -1, room_.id_players[*(room_.participants_it)], "game over", 0, 0 ) );

      if(phase == 1){  goto done; }

    }
  }

  /******************/
  /*   Tie Breaker  */
  /******************/

  else if(game == -2)
  {
    std::cout << "tie - hand value to store: " << info["hand value"] << std::endl;
    room_.hand_values[ info["player_id"].get<std::string>() ] = info.value("hand value", 0);
    phase--;
    turn_index++;
    std::cout << "tie - hand value stored" << std::endl;
    room_.participants_it = room_.participants().begin();
    std::advance(room_.participants_it, turn_index);
    room_.deliver( turn( -2, room_.id_players[*(room_.participants_it)], "game over", idx_card, 0 ) );

    if(phase == 1){  goto done; }

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
  to_player["amount"] = _dg.getPot();

  json_str = to_player.dump();

  msg.body_length(std::strlen(json_str.c_str()));
  std::memcpy(msg.body(), json_str.c_str(), msg.body_length());
  msg.encode_header();

  return msg;
}

//method that returns the total money in the pot
int Dealer_Game::getPot()
{
  return _pot.getCurrentPot();
}

//method that connects a deck with the dealer
void Dealer_Game::setDeck(Deck _deck)
{
  this->_deck = _deck;
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
chat_message Dealer_Game::exchangeCards(int amountOfCards, std::string id_player)
{
  nlohmann::json to_player;
  chat_message cards;
  std::string json_str;

  to_player["event"] = "Deal";
  to_player["id_player"] = id_player;
  to_player["cards_requested"] = amountOfCards;

  for(int i = 0; i < amountOfCards; i++)
  {
    std::string in = std::to_string(i+1);
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
    io_context.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }
  return 0;
}
