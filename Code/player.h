#ifndef PLAYER_COMM_H
#define PLAYER_COMM_H

#include <assert.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <cstdlib>
#include <deque>
#include <gtkmm.h>
#include <iostream>
#include <vector>

#include "asio.hpp"
#include "chat_message.hpp"
#include "hand.h"
#include "json.hpp"
#include "Mainwin.h"
#include "stack.h"


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

    player_comm();
    void setName(std::string name);
    int getChipAmount();
    std::vector<std::string> setHand(nlohmann::json cards, int cardNum);
    chat_message move_j(std::string play, int cards_requested, int current_bet);
    chat_message exchange_j(std::string play, int cards_requested, std::vector<int> cards);
    int turn; // to show proper buttons, -1 = out of game, 0 = not your turn, 1 = ante, 2 = exchange button greyed out		

  private:
    void do_connect(const tcp::resolver::results_type& endpoints);
    void do_read_header();
    void do_read_body();
    void do_write();

  private:
    asio::io_context& io_context_;
    std::string _id; //attribute that represents the player id
    tcp::socket socket_;
    chat_message read_msg_;
    chat_message_queue write_msgs_;
    Mainwin* _win; //main window attribute

    std::string _name; //attribute that represents the player's name
    Stack _stack; //attribute that represents the stack of chips
    Hand _hand; //attribute that represents the player's hand
  
};		
/*
class Player_Game
{
  public:


  private:

};
*/
#endif
