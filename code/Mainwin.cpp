#include "Mainwin.h"
#include <dirent.h>
#include <fstream>
#include <experimental/filesystem>

Mainwin::Mainwin(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) : Gtk::Window(cobject), builder{refGlade} {
	set_title("Poker ++");

	builder->get_widget("fold_button", fold_button);
	builder->get_widget("check_button", check_button);
	builder->get_widget("bet_button", bet_button);
	builder->get_widget("ante_button", ante_button);
	builder->get_widget("bet_entry", bet_entry);
	builder->get_widget("msg", msg);
	builder->get_widget("bet_label", bet_label);
	builder->get_widget("playername_label", playername_label);
	builder->get_widget("MenuBar", MenuBar);
	builder->get_widget("menuitem_help", menuitem_help);
	builder->get_widget("menuitem_about", menuitem_about);
	builder->get_widget("menuitem_file", menuitem_file);
	builder->get_widget("menuitem_quit", menuitem_quit);


	menuitem_about->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_about_click));

	menuitem_quit->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_quit_click));

	fold_button->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_fold_click));

	check_button->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_check_click));

	bet_button->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_bet_click));

	ante_button->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_ante_click));
	

}

Mainwin::~Mainwin() {}

void Mainwin::on_quit_click() {
	/* Alert the dealer that a player/spectator has left the game */
	close();
}

void Mainwin::on_about_click() { 
	Gtk::AboutDialog dialog{};    
	dialog.set_transient_for(*this);    
	dialog.set_program_name("Poker++");    
	dialog.set_version("Version 1.1.0");    
	dialog.set_copyright("Copyright 2020");    
	dialog.set_license_type(Gtk::License::LICENSE_GPL_3_0);    
	std::vector< Glib::ustring > authors = {"*list authors*"};    
	dialog.set_authors(authors);    
	dialog.run(); 
	}

void Mainwin::on_check_click() {
	/* First, the method should check if any player has bet yet
	 * if they have, the player must choose a different option
	 * if they have not, a JSON will be sent to the dealer 
	 * and it will move on to the next player's turn */
}

void Mainwin::on_bet_click() {
	/* This method should take the text from bet_entry and attempt to convert it to a long
	 * If this fails, this was an invalid input and the player must enter something
	 * different
	 * Next, the method should check if the bet entered is lower than the current bet
	 * If it is lower, the player must enter something different
	 * Once the entry is correct, a JSON will be sent to the dealer 
	 * and it will move on to the next player's turn */
}

void Mainwin::on_fold_click() {
	/* This method will simply send a JSON to the dealer and will not be allowed to
	 * particpate in the current game
	 * The player will spectate the rest of the game until it ends or the window is
	 * closed */
}

void Mainwin::on_ante_click() {
	/* This method will only be used at the begnning of the game
	 * It will have almost the same funcitonality as the bet button
	 * Once a player has ante'd the correct amount, it will send a JSON to dealer
	 * and move on to the next player's turn */
}
