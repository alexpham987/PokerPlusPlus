#include "dealer_comm.h"

using asio::ip::tcp;


//Class chat_room

//adds player to game
void chat_room::join(chat_participant_ptr participant)
{
  std::cout << "participant joined" << std::endl;
  participants_.insert(participant);		
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


//Class chat_session

chat_session::chat_session(tcp::socket socket, chat_room& room)
  : socket_(std::move(socket)),
    room_(room)
{
  //_dg.setDeck(_deck);
}

//starts game session
void chat_session::start()
{
  room_.join(shared_from_this());
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
  std::cout << "reading message" << std::endl;
  auto self(shared_from_this());
  asio::async_read(socket_,
      asio::buffer(read_msg_.body(), read_msg_.body_length()),
      [this, self](std::error_code ec, std::size_t /*length*/)
      {
        if (!ec)
        {
          nlohmann::json info = nlohmann::json::parse(read_msg_.body());
          chat_message msg;
	  if(info["event"] == "join")
	  {
	    chat_message cards = _dg.dealCards(5);
	    room_.deliver(cards);
	  }
	  else if(info["event"] == "ante")
	  {
	    _dg.addMoney(1);
	    std::cout << "chip added" << std::endl;
            room_.deliver(read_msg_);
	  }
	  else if(info["event"] == "bet" ||info["event"] == "call" || info["event"] == "raise") 
	  {
	    int amount = info["current_bet"];
	    _dg.addMoney(amount);
	    std::cout << "chip added" << std::endl;
            room_.deliver(read_msg_);
	  }
	  else if(info["event"] == "request_cards")
	  {
	    std::cout << "getting cards" << std::endl;
	    int num = info["cards_requested"];
	    chat_message cards = _dg.dealCards(num);
	    room_.deliver(cards);
	  }
	  else 
          {
            room_.deliver(read_msg_);
	  }
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


//Class Dealer_comm

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
          std::make_shared<chat_session>(std::move(socket), room_)->start();
        }


        do_accept();
      });
}

