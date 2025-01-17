#include "Mainwin.h"
#include "player.h"
#include "deck.h"
#include "pot.h"

player_comm::player_comm(asio::io_context& io_context, const tcp::resolver::results_type& endpoints)
   : io_context_(io_context), socket_(io_context) 
{
        _id = "0";
	do_connect(endpoints);
}

//sets the main window attribute
void player_comm::setMainwin(Mainwin* win) 
{
  _win = win;
}

//writes chat messages to dealer
void player_comm::write(const chat_message& msg)
{
  asio::post(io_context_,
      [this, msg]()
      {
        bool write_in_progress = !write_msgs_.empty();
        write_msgs_.push_back(msg);
        if (!write_in_progress)
        {
          do_write();
        }
   	 });
}

//closes player
void player_comm::close()
{
  asio::post(io_context_, [this]() { socket_.close(); });
}

//connects player to dealer
void player_comm::do_connect(const tcp::resolver::results_type& endpoints)
{
  asio::async_connect(socket_, endpoints,
      [this](std::error_code ec, tcp::endpoint)
      {
        if (!ec)
        {
          do_read_header();
        }
      });
}

//reads chat message header
void player_comm::do_read_header()
{
  asio::async_read(socket_,
      asio::buffer(read_msg_.data(), chat_message::header_length),
      [this](std::error_code ec, std::size_t /*length*/)
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
          socket_.close();
        }
      });
} 

//reads chat message body
void player_comm::do_read_body()
{
  asio::async_read(socket_,
      asio::buffer(read_msg_.body(), read_msg_.body_length()),
      [this](std::error_code ec, std::size_t /*length*/)
      {
        if (!ec)
        {
          char outline[read_msg_.body_length() + 2];
                                     // '\n' + '\0' is 2 more chars
          outline[0] = '\n';
          outline[read_msg_.body_length() + 1] = '\0';
          std::memcpy ( &outline[1], read_msg_.body(), read_msg_.body_length() );

          std::cout << "parsing " << read_msg_.body() << std::endl;
          nlohmann::json info = nlohmann::json::parse(read_msg_.body());
          std::cout << "event = " << info["event"] << std::endl;


          if( this->_id == "0" )
          {
            this->updateLabel(info);
          }

          else if(info["event"] == "winner")
          {
            if(info["id_player"] == this -> _id)
            {
              std::cout << "-------------------I won!" << std::endl;
              info["won"] = "yes";
              this->updateLabel(info);
            }
            else
            {
              std::cout << "**************** I lost :(" << std::endl;
              info["won"] = "no";
              this->updateLabel(info);
            }
          }

          else if(info["event"] == "Deal")
          {
            if(info["id_player"] == this->_id)
            {
              int num = info["cards_requested"];
              _win->setCards(info, num);
              this->updateLabel(info);
              _win->my_turn_first();
              if(num == 5) 
              {
                write( move_j("dealt", 0, 0) );
              }
              else
              {
                write( move_j("done", 0, 0) );
              }
              
            }
            else
            {
              _win->wait_for_turn();
            }
          }
          else if(info["event"] == "turn")
          {
            if(info["id_player"] == this->_id)
            {
              if(info["turn"] == 1)
              {
                turn = 1;
                _win->my_turn();
              }
              if(info["turn"] == 2)
              {
                _win->my_turn_second();
              }
              if(info["turn"] == 3)
              {
                _win->my_turn_third();
              }
              if(info["turn"] == 4)
              {
                _win->my_turn_second();
              }
              this->updateLabel(info);
            }

            else if(info["event"] == "bet" || info["event"] == "raise")
            {
              if(info["id_player"] != this->_id)
              {
                this->updateLabel(info);
              }
            }

            else
            {
              turn = 0;
              _win->wait_for_turn();
              this->updateLabel(info);
            }
          }
          else if(info["event"] == "game over")
          {
            _win->wait_for_turn();
            if(info["id_player"] == this->_id)
            {
              write( move_j( "hand value", -1, _hand.calc_value() ) );
            }
          }
          else if(info["event"] == "tie")
          {
            _win->wait_for_turn();
            if(info["id_player"] == this->_id)
            {
              write( move_j( "hand value", -1, _hand.find_next_highest() ) );
            }
          }
          do_read_header();
        }
        else
        {
          socket_.close();
        }
      });
}

//writes chat message
void player_comm::do_write()
{
  asio::async_write(socket_,
      asio::buffer(write_msgs_.front().data(),
        write_msgs_.front().length()),
      [this](std::error_code ec, std::size_t /*length*/)
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
          socket_.close();
        }
      });
}
	
//updates label in the main window
void player_comm::updateLabel(nlohmann::json info) 
{

  std::string lab;

  if(info["event"] == "set_id")
  {
    _id = info["id_player"];
    lab = "id set";
  }

  else if(info["event"] == "Deal")
  {
    lab = "Cards Dealt!";
    _win->setLabel(lab);
  }

  else if(info["event"] == "winner")
  {
    if(info["won"] == "no")
    {
      lab = "You lost";
    }
    else
    {
      int amount = info["amount"];
      lab = "You won $" + std::to_string(amount) + ", congrats!";
    }
    _win->setLabel(lab);
  }

  else
  {
    //std::string name = info["name"];
    std::string event = info["move"];
    //lab = name+"'s move:  ";

    if(event == "bet" || event == "raise" || event == "call") 
    {
      std::string name = "name";
      int bet = info["current_bet"];
      std::string bet_str = std::to_string(bet);
      lab = name+"'s move:  ";
      lab += event+" $"+bet_str;
      _win->set_bet(bet);
    }
    else if(event == "request_cards")
    {
      std::string name = "name";
      int c = info["cards_requested"];
      std::string c_str = std::to_string(c);
      lab = name+"'s move:  ";
      lab += "exchanged "+c_str+" cards";
    }
    else
    {
      lab += event;
    }
    _win->setLabel(lab);
  }
}

//method that updates the _name attribute with the passed in parameter
void player_comm::setName(std::string name)
{
  _name = name;
}

int player_comm::getChipAmount() 
{
  return _stack.get_total();
}

//method that sets the player's hand
std::vector<std::string> player_comm::setHand(nlohmann::json cards, int cardNum)
{
  std::vector<std::string> files;
  int num;
  std::string s;
  Suit suit;

  //checks for the amount of cards and adds the correct cards
  for(int i = 1; i <= cardNum; i++)
  {
    std::stringstream ss;
    std::string in = std::to_string(i);
    std::string cStr = cards[in];
    ss << cStr;
    ss >> num;
    ss >> s;

    if(s == "S")
      suit = S;
    else if(s == "C")
      suit = C;
    else if(s == "D")
      suit = D;
    else
      suit = H;

    Card c(num, suit);
    _hand.addCard(c);
  }

  std::vector<Card> hand = _hand.getHand();
  for(auto card : hand)
  {
    files.push_back(card.card_to_filename());
  }

  return files;
}


//method that returns the move that a player chooses
chat_message player_comm::move_j(std::string play, int cards_requested, int current_bet)
{
  nlohmann::json to_dealer;
  chat_message msg;
  std::string json_str;

  to_dealer["player_id"] = this->_id;
  to_dealer["name"] = this->_name;
  to_dealer["event"] = play;        // "stand","hit","fold","raise","join","request_cards"
  to_dealer["cards_requested"] = cards_requested; //optional, number of cards requested, 1 to 5
  to_dealer["current_bet"] = current_bet;
  if(cards_requested == -1)
  {
    to_dealer["hand value"] = current_bet;
    to_dealer["high_card"] = _hand.find_high_card();
  }

  json_str = to_dealer.dump();

  msg.body_length(std::strlen(json_str.c_str()));
  std::memcpy(msg.body(), json_str.c_str(), msg.body_length());
  msg.encode_header();
  
  if(play == "ante")
  {
    _stack.remove_chips(0,1,0);
  }
    
  //deals with chips correctly when there is a bet, ante, or raise
  if(play == "bet" || play == "call" || play == "raise")
  {
    int blue = current_bet/25;
    current_bet -= 25*blue;

    int green = current_bet/5;
    current_bet -= 5*green;

    int red = current_bet;

    _stack.remove_chips(green,red,blue);
  }

  return msg;
}

//method that exchanges the cards in a player's hand
chat_message player_comm::exchange_j(std::string play, int cards_requested, std::vector<int> cards)
{
  _hand.modify_hand(cards);

  nlohmann::json to_dealer;
  chat_message msg;
  std::string json_str;

//  to_dealer["uuid"] = this->_id;
  to_dealer["name"] = this->_name;
  to_dealer["event"] = "request_cards"; // "stand","hit","fold","raise","join","request_cards"
  to_dealer["cards_requested"] = cards_requested; //optional, number of cards requested, 1 to 5
  to_dealer["current_bet"] = 0;

  json_str = to_dealer.dump();

  msg.body_length(std::strlen(json_str.c_str()));
  std::memcpy(msg.body(), json_str.c_str(), msg.body_length());
  msg.encode_header();

  return msg;
}


//main function for the player's side of the game
int main(int argc, char *argv[])
{
  asio::io_context io_context;

  tcp::resolver resolver(io_context);
  auto endpoints = resolver.resolve(argv[1], argv[2]);

  player_comm* p = new player_comm(io_context, endpoints);
  assert(p);

  std::thread t([&io_context](){ io_context.run(); });

  Gtk::Main kit(argc, argv);
  Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("PokerPP.glade");
  Mainwin *win = 0;
  builder->get_widget_derived("Mainwin", win);

  //player_comm pg;
  p->setMainwin(win);

  //win->setPlayerGame(pg);
  win->setPlayerComm(p);

  kit.run(*win);

  p->close();

  t.join();
  
  return 0;
}
