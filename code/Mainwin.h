#ifndef __MAINWIN_H
#define __MAINWIN_H

// include other classes
#include <gtkmm.h>
#include <string>
#include <exception>
#include <iostream>
#include <sstream>
#include "json.hpp"
#include "chat_message.hpp"
#include "player_comm.h"

class Mainwin : public Gtk::Window
{
    public:
        Mainwin(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& reGlade);
        virtual ~Mainwin();
    protected:        
        void on_quit_click();  
	void on_check_click();
	void on_bet_click();
	void on_fold_click();
	void on_ante_click();
	void on_about_click();
		
    private: 
	Gtk::MenuBar* MenuBar;
	Gtk::MenuItem* menuitem_help;
	Gtk::MenuItem* menuitem_about; 
	Gtk::MenuItem* menuitem_file;
	Gtk::MenuItem* menuitem_quit;                                             
        Gtk::Label* msg;
	Gtk::Label* playername_label;
	Gtk::Label* bet_label;
	Gtk::Button* fold_button;
	Gtk::Button* check_button;
	Gtk::Button* bet_button;
	Gtk::Button* ante_button;
	Gtk::Entry* bet_entry;
	Glib::RefPtr<Gtk::Builder> builder;

};
#endif 
