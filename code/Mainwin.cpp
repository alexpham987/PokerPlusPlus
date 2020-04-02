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
	

	Gtk::Dialog *dialog = new Gtk::Dialog();
	dialog->set_transient_for(*this);
	dialog->set_title("Enter Name");
	Gtk::HBox b_name;

	Gtk::Entry e_name;
	e_name.set_max_length(50);
	b_name.pack_start(e_name, Gtk::PACK_SHRINK);
	dialog->get_vbox()->pack_start(b_name, Gtk::PACK_SHRINK);

	dialog->add_button("Enter", 0);
	dialog->show_all();
	int result = 1;
	std::string player_name;

	while(result) {
		result = dialog->run();
		player_name = e_name.get_text();
		playername_label->set_label(player_name);
	}
	dialog->close();
	delete dialog;

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
	std::vector< Glib::ustring > authors = {"\nBailey Brown \nAlex Pham \nMarcos Juarez"};    
	dialog.set_authors(authors);    
	dialog.run(); 
	}

void Mainwin::on_check_click() {
	//check if any player has bet yet

	chat_message msg;

	nlohmann::json to_dealer;
	to_dealer["decision"] = "check";
	to_dealer["name"] = "name";
	to_dealer["uuid"] = "xyz";

	std::string json_str = to_dealer.dump();

	msg.body_length(std::strlen(json_str.c_str()));
	std::memcpy(msg.body(), json_str.c_str(), msg.body_length());
	msg.encode_header();

	//player_comm.write(msg);
}

void Mainwin::on_bet_click() {
	//Check if bet_entry->get_text() is valid
	int bet_amount = std::stoi(bet_entry->get_text());

	chat_message msg;
	nlohmann::json to_dealer;

	//if there is no current bet
		to_dealer["decision"] = "bet";
	//else if bet_amount < current bet
		std::cout << "error" << std::endl;
	//else if bet_amount > current bet amount
		to_dealer["decision"] = "raise";
	//else
		to_dealer["decision"] = "call";

	to_dealer["bet"] = bet_amount;
	to_dealer["name"] = "name";
	to_dealer["uuid"] = "xyz";

	std::string json_str = to_dealer.dump();

	msg.body_length(std::strlen(json_str.c_str()));
	std::memcpy(msg.body(), json_str.c_str(), msg.body_length());
	msg.encode_header();
	//player_comm.write(msg);
}

void Mainwin::on_fold_click() {

	chat_message msg;

	nlohmann::json to_dealer;
	to_dealer["decision"] = "fold";
	to_dealer["name"] = "name";
	to_dealer["uuid"] = "xyz";

	std::string json_str = to_dealer.dump();

	msg.body_length(std::strlen(json_str.c_str()));
	std::memcpy(msg.body(), json_str.c_str(), msg.body_length());
	msg.encode_header();

	//player_comm.write(msg);
	
	//Allow player to spectate	
}

void Mainwin::on_ante_click() {
	//Check if bet_entry->get_text() is valid
	int ante_amount = std::stoi(bet_entry->get_text());

	chat_message msg;

	nlohmann::json to_dealer;
	to_dealer["decision"] = "ante";
	to_dealer["amount"] = ante_amount;
	to_dealer["name"] = "name";
	to_dealer["uuid"] = "xyz";

	std::string json_str = to_dealer.dump();

	msg.body_length(std::strlen(json_str.c_str()));
	std::memcpy(msg.body(), json_str.c_str(), msg.body_length());
	msg.encode_header();

	//player_comm.write(msg);
}
