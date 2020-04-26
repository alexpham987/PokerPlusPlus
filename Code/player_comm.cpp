#include "player_comm.h"

player_comm::player_comm(asio::io_context& io_context, const tcp::resolver::results_type& endpoints)
   : io_context_(io_context), socket_(io_context) 
{
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
          this->updateLabel(info);

          if(info["event"] == "Deal")
          {
            std::cout << "dealing" << std::endl;
            int num = info["cards_requested"];
            _win->setCards(info, num);
          }

          std::cout.write(read_msg_.body(), read_msg_.body_length());
          std::cout << "\n";
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

  if(info["event"] == "Deal")
    lab = "Cards Dealt!";

  else if(info["event"] == "collect_ante")
    lab = "Please enter ante!";

  else
  {
    std::string name = info["name"];
    std::string event = info["event"];
    lab = name+"'s move:  ";

    if(event == "bet" || event == "raise" || event == "call") 
    {
      int bet = info["current_bet"];
      std::string bet_str = std::to_string(bet);
      lab += event+" $"+bet_str;
    }
    else if(event == "request_cards")
    {
      int c = info["cards_requested"];
      std::string c_str = std::to_string(c);
      lab += "exchanged "+c_str+" cards";
    }
    else
    {
      lab += event;
    }
  }
  _win->setLabel(lab);
}


