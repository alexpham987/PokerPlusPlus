#ifndef PLAYER_COMM_H
#define PLAYER_COMM_H

#include <iostream>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <assert.h>
#include "asio.hpp"
#include "chat_message.hpp"
#include "json.hpp"
#include "Mainwin.h"
#include <gtkmm.h>

class Mainwin;

using asio::ip::tcp;
typedef std::deque<chat_message> chat_message_queue;

class player_comm
{
	public:
		player_comm(asio::io_context& io_context, const tcp::resolver::results_type& endpoints);
		void write(const chat_message& msg);
		void close();
		std::string message();
		void updateLabel(nlohmann::json info);
		void setMainwin(Mainwin* win);

	private:
		void do_connect(const tcp::resolver::results_type& endpoints);
		void do_read_header();
		void do_read_body();
		void do_write();

	private:
		asio::io_context& io_context_;
  	tcp::socket socket_;
  	chat_message read_msg_;
  	chat_message_queue write_msgs_;
		Mainwin* _win;
};

#endif
