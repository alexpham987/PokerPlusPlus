#ifndef __MAINWIN_H
#define __MAINWIN_H

#include <gtkmm.h>
#include <string>
#include <exception>
#include <iostream>
#include <sstream>
#include <vector>
#include "json.hpp"
#include "chat_message.hpp"
#include "player_comm.h"
#include "player_game.h"

class player_comm;

//inherits from Gtk::Window to use gtk methods
class Mainwin : public Gtk::Window
{
  public:
    Mainwin(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& reGlade);
    virtual ~Mainwin();
    void setPlayerGame(Player_Game pgame);
    void setPlayerComm(player_comm* pcomm);
    void setLabel(std::string text);
    void setCards(nlohmann::json cards, int num);

  protected:
    void on_quit_click();
    void on_check_click();
    void on_bet_click();
    void on_fold_click();
    void on_ante_click();
    void on_stand_click();
    void on_exchange_click();
    void on_about_click();

  private:
    Gtk::MenuBar* MenuBar; //  represents the menu bar on the gtk window
    Gtk::MenuItem* menuitem_help; //  represents help on the menu bar
    Gtk::MenuItem* menuitem_about; //  represents about on the menu bar
    Gtk::MenuItem* menuitem_file; //  represents file on the menu bar
    Gtk::MenuItem* menuitem_quit; //  represents quit on the menu bar
    Gtk::Label* msg; //represents the message that gets updated throughout the game (right below the player name)
    Gtk::Label* playername_label; //  represents the player name
    Gtk::Label* bet_label; //  represents the string "Enter Amount:" on the gtk window
    Gtk::Button* fold_button; //  represents the fold button
    Gtk::Button* check_button; //  represents the check button
    Gtk::Button* bet_button; //  represents the bet button
    Gtk::Button* ante_button; //  represents the ante button
    Gtk::Button* stand_button; // represents stand button
    Gtk::Entry* bet_entry; //  represents the entry box to enter the betting amount
    Gtk::Button* exchange_button; //  represents the exchange cards button
    Gtk::Fixed *fixed; //  represents the background of the gtk window
    Gtk::Box *card_box; //  represents the box that contains the cards
    Gtk::Label* chip1_label; //  represents the dollar value of the blue chip
    Gtk::Label* chip2_label; //  represents the dollar value of the green chip
    Gtk::Label* chip3_label; //  represents the dollar value of the red chip
    Gtk::Image* card_1; //  represents the 1st card
    Gtk::Image* card_2; //  represents the 2nd card
    Gtk::Image* card_3; //  represents the 3rd card
    Gtk::Image* card_4; //  represents the 4th card
    Gtk::Image* card_5; //  represents the 5th card
    Glib::RefPtr<Gtk::Builder> builder; //  represents the vector of gtk builders

  private:
    std::string _player_name;
    Player_Game _pg;
    player_comm* _pc;
    int current_bet;
};

#endif
