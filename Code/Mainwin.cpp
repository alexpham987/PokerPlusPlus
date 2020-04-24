#include "Mainwin.h"
#include <dirent.h>
#include <fstream>

Mainwin::Mainwin(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) : Gtk::Window(cobject), builder{refGlade} {
	set_title("Poker ++");


	builder->get_widget("fixed", fixed);
	builder->get_widget("fold_button", fold_button);
	builder->get_widget("check_button", check_button);
	builder->get_widget("bet_button", bet_button);
	builder->get_widget("ante_button", ante_button);
	builder->get_widget("bet_entry", bet_entry);
	builder->get_widget("msg", msg);
	builder->get_widget("bet_label", bet_label);
	builder->get_widget("playername_label", playername_label);
	builder->get_widget("chip1_label", chip1_label); //added chip1_label
	builder->get_widget("chip2_label", chip2_label); //added chip2_label
	builder->get_widget("chip3_label", chip3_label); //added chip3_label
	builder->get_widget("MenuBar", MenuBar);
	builder->get_widget("menuitem_help", menuitem_help);
	builder->get_widget("menuitem_about", menuitem_about);
	builder->get_widget("menuitem_file", menuitem_file);
	builder->get_widget("menuitem_quit", menuitem_quit);
	//Excange button will need to be toggled ON during exchange cards round
	builder->get_widget("exchange_button", exchange_button);
	//exchange_button->set_sensitive(true);
	builder->get_widget("card_box", card_box);
	builder->get_widget("card_1", card_1);


	menuitem_about->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_about_click));

	menuitem_quit->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_quit_click));

	fold_button->signal_clicked().connect(sigc::mem_fun(*this, &Mainwin::on_fold_click));

	check_button->signal_clicked().connect(sigc::mem_fun(*this, &Mainwin::on_check_click));

	bet_button->signal_clicked().connect(sigc::mem_fun(*this, &Mainwin::on_bet_click));

	ante_button->signal_clicked().connect(sigc::mem_fun(*this, &Mainwin::on_ante_click));

	exchange_button->signal_clicked().connect(sigc::mem_fun(*this, &Mainwin::on_exchange_click));

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

	while(result) {
		result = dialog->run();
		_player_name = e_name.get_text();
		playername_label->set_label(_player_name);
	}
	dialog->close();
	delete dialog;
}

Mainwin::~Mainwin() {}

void Mainwin::setPlayerGame(Player_Game pgame) { 
	_pg = pgame; 
	_pg.setName(_player_name);
}

void Mainwin::setPlayerComm(player_comm* pcomm) {
	_pc = pcomm;
	chat_message join = _pg.move_j("join", 0, 0);
	_pc->write(join);
}

void Mainwin::setLabel(std::string text) 
{
	msg->set_label(text);
}

void Mainwin::setCards(nlohmann::json cards)
{
	Card c(2,C);
	Card c1(3,D);
	card_1->set(c.card_to_filename());
}
	

void Mainwin::on_quit_click() {
	/* Alert the dealer that a player/spectator has left the game */
	close();
}

void Mainwin::on_about_click() { 
	std::cout << "about pressed" << std::endl;
	Gtk::AboutDialog dialog{};    
	dialog.set_transient_for(*this);    
	dialog.set_program_name("Poker++");    
	dialog.set_version("Version 1.1.0");    
	dialog.set_copyright("Copyright 2020");    
	dialog.set_license_type(Gtk::License::LICENSE_GPL_3_0);    
	std::vector< Glib::ustring > authors = {"Bailey Brown \nAlex Pham \nMarcos Juarez"};    
	dialog.set_authors(authors);    
	dialog.run();
} 

void Mainwin::on_check_click() {
	std::cout << "check button clicked" << std::endl;
	//check if any player has bet yet

	chat_message info = _pg.move_j("check", 0, 0);
	_pc->write(info);
}

void Mainwin::on_bet_click() {
	std::cout << "bet button pressed" << std::endl;
	int bet_amount;

	try {
		bet_amount = std::stoi(bet_entry->get_text());
	} catch(std::exception e) {
		bet_entry->set_text("### Invalid ###");
		return;
	}
	
	chat_message info = _pg.move_j("bet", 0, bet_amount);
	_pc->write(info);

}

void Mainwin::on_fold_click() {
	std::cout << "fold button pressed" << std::endl;

	chat_message info = _pg.move_j("fold", 0, 0);
	_pc->write(info);	
}

	
void Mainwin::on_ante_click() {
	std::cout << "ante button clicked" << std::endl;
		int ante_amount;
	try {
		ante_amount = std::stoi(bet_entry->get_text());
	} catch(std::exception e) {
		bet_entry->set_text("### Invalid ###");
		return;
	}

	chat_message info = _pg.move_j("ante", 0, ante_amount);
	_pc->write(info);
}

void Mainwin::on_exchange_click() {
	std::cout << "exchange button pressed" << std::endl;

	Gtk::Dialog *dialog = new Gtk::Dialog();
	dialog->set_transient_for(*this);
	dialog->set_title("Enter Card #s (1-5)");
	Gtk::HBox b_cards;

	Gtk::Entry e_cards;
	e_cards.set_max_length(50);
	b_cards.pack_start(e_cards, Gtk::PACK_SHRINK);
	dialog->get_vbox()->pack_start(b_cards, Gtk::PACK_SHRINK);

	dialog->add_button("Enter", 0);
	dialog->show_all();
	int result = 1;
	std::string exchange_cards;

	while(result) {
		result = dialog->run();
		exchange_cards = e_cards.get_text();
		std::cout << exchange_cards << std::endl;
	}
	dialog->close();
	delete dialog;	

	std::vector<int> cards;
	std::stringstream ss(exchange_cards);
	int num;
	while(ss >> num)
		cards.push_back(num);

	chat_message info = _pg.exchange_j("request_cards", cards.size(), cards);
	_pc->write(info);
		
}


