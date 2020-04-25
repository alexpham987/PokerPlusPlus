#include "dealer_comm.h"

using asio::ip::tcp;

//Class chat_room

void chat_room::join(chat_participant_ptr participant)
{
	std::cout << "participant joined" << std::endl;
  participants_.insert(participant);
  for (auto msg: recent_msgs_)
    participant->deliver(msg);
}

void chat_room::leave(chat_participant_ptr participant)
{
  participants_.erase(participant);
}

void chat_room::deliver(const chat_message& msg)
{
  recent_msgs_.push_back(msg);
  while (recent_msgs_.size() > max_recent_msgs)
    recent_msgs_.pop_front();
  // CSE3310 messages are sent to all connected clients
  for (auto participant: participants_)
    participant->deliver(msg);
}

std::set<chat_participant_ptr> chat_room::participants()
{
	return participants_;
}

//Class chat_session

chat_session::chat_session(tcp::socket socket, chat_room& room)
  :socket_(std::move(socket)),
  room_(room)
{
}

void chat_session::start()
{
  room_.join(shared_from_this());
  do_read_header();
}

void chat_session::deliver(const chat_message& msg)
{
  bool write_in_progress = !write_msgs_.empty();
  write_msgs_.push_back(msg);
  if (!write_in_progress)
  {
    do_write();
  }
}

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

void chat_session::do_read_body()
{
  std::cout << "reading message" << std::endl;
  auto self(shared_from_this());
  asio::async_read(socket_,
    asio::buffer(read_msg_.body(), read_msg_.body_length()),
    [this, self](std::error_code ec, std::size_t /*length*/)
    {
      if(!ec)
      {
        nlohmann::json info = nlohmann::json::parse(read_msg_.body());
			  chat_message msg;
			  if(info["event"] == "join")
			  {
				  chat_message cards = _dg.dealCards();
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
  :acceptor_(io_context, endpoint)
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
      std::make_shared<chat_session>(std::move(socket), room_)->start();
    }
    do_accept();
  });
}
