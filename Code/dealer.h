#ifndef DEALER_H
#define DEALER_H

#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <chrono>
#include <vector>
#include <string>
#include "asio.hpp"
#include "chat_message.hpp"
#include "pot.h"
#include "deck.h"
#include "json.hpp"
#include "chat_message.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

using asio::ip::tcp;

//----------------------------------------------------------------------

typedef std::deque<chat_message> chat_message_queue;

//----------------------------------------------------------------------

class chat_participant
{
  public:
    virtual ~chat_participant() {}
    virtual void deliver(const chat_message& msg) = 0;
};

typedef std::shared_ptr<chat_participant> chat_participant_ptr;

class chat_room
{
  public:
    void join(chat_participant_ptr participant, std::string/*boost::uuids::uuid*/);
    void leave(chat_participant_ptr participant);
    void deliver(const chat_message& msg);
    std::set<chat_participant_ptr> participants();
    std::map<chat_participant_ptr, std::string/*boost::uuids::uuid*/> id_players;
    std::set<chat_participant_ptr>::iterator participants_it;
    std::map<chat_participant_ptr, std::string> name_players;
    std::map<std::string, int> hand_values;

  private:	
    std::set<chat_participant_ptr> participants_; //set of players
    enum { max_recent_msgs = 100 }; //max number of messages
    chat_message_queue recent_msgs_; //queue of messages
};

class Dealer_Game
{
  public:
    Dealer_Game(Deck deck);
    void shuffleCards();
    chat_message dealCards(int num, std::string id);
    bool gameResult();
    chat_message exchangeCards(int amountOfCards);
    void revealHand();
    void addMoney(int amount);
    chat_message turn(int, boost::uuids::uuid);
    void startGame();
    void setDeck(Deck);

  private:
    bool _playerResult; //attribute that represents whether a player has won or lost
    Deck _deck; //attribute that represents the deck of cards
    Pot _pot{0}; //attribute that represents the pot for the game
};

class Dealer_comm
{
  public:
    Dealer_comm(asio::io_context& io_context,
	const tcp::endpoint& endpoint);
    void startGame();
    void write(const chat_message& msg);

  private:
    void do_accept();
    tcp::acceptor acceptor_;
    chat_room room_;

    friend class chat_session;
};

class chat_session 
  : public chat_participant, public std::enable_shared_from_this<chat_session>
{
  public:
    chat_session(tcp::socket socket, chat_room& room);
    void start(Dealer_comm* _dc);
    void deliver(const chat_message& msg);
    void startGame(Dealer_comm* _dc);
    chat_message turn(int, std::string/*boost::uuids::uuid, std::string*/, std::string, int, int);
    //chat_message turn(int, std::string/*boost::uuids::uuid*/, int);
    chat_message set_id(std::string/*boost::uuids::uuid*/);
    void do_round(nlohmann::json info);
    chat_message winner(std::string, int);


  private:
    void do_read_header();
    void do_read_body();
    void do_write();
    tcp::socket socket_;
    chat_room& room_;
    chat_message read_msg_;
    chat_message_queue write_msgs_;
    //Deck _deck;
    //Dealer_Game _dg{_deck};
    int joined;
    
    friend class Dealer_Game;
};




	
#endif
