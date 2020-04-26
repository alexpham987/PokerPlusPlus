#include "Mainwin.h"
#include <dirent.h>
#include <fstream>

//constructor for the Mainwin class (it has code for the gtk window)
Mainwin::Mainwin(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) : Gtk::Window(cobject), builder{refGlade}
{
  //title of the game
  set_title("Poker ++");

  //all of the things that you see on the gtk window for the poker game need to be set-up
  builder->get_widget("fixed", fixed);
  builder->get_widget("fold_button", fold_button);
  builder->get_widget("check_button", check_button);
  builder->get_widget("bet_button", bet_button);
  builder->get_widget("ante_button", ante_button);
  builder->get_widget("stand_button", stand_button);
  builder->get_widget("bet_entry", bet_entry);
  builder->get_widget("msg", msg);
  builder->get_widget("bet_label", bet_label);
  builder->get_widget("playername_label", playername_label);
  builder->get_widget("chip1_label", chip1_label);
  builder->get_widget("chip2_label", chip2_label); 
  builder->get_widget("chip3_label", chip3_label); 
  builder->get_widget("MenuBar", MenuBar);
  builder->get_widget("menuitem_help", menuitem_help);
  builder->get_widget("menuitem_about", menuitem_about);
  builder->get_widget("menuitem_file", menuitem_file);
  builder->get_widget("menuitem_quit", menuitem_quit);
  builder->get_widget("exchange_button", exchange_button);
  builder->get_widget("card_box", card_box);
  builder->get_widget("card_1", card_1);
  builder->get_widget("card_2", card_2);
  builder->get_widget("card_3", card_3);
  builder->get_widget("card_4", card_4);
  builder->get_widget("card_5", card_5);

  //deals with clicking the buttons on the gtk window
  menuitem_about->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_about_click));
  menuitem_quit->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_quit_click));
  fold_button->signal_clicked().connect(sigc::mem_fun(*this, &Mainwin::on_fold_click));
  check_button->signal_clicked().connect(sigc::mem_fun(*this, &Mainwin::on_check_click));
  bet_button->signal_clicked().connect(sigc::mem_fun(*this, &Mainwin::on_bet_click));
  ante_button->signal_clicked().connect(sigc::mem_fun(*this, &Mainwin::on_ante_click));
  exchange_button->signal_clicked().connect(sigc::mem_fun(*this, &Mainwin::on_exchange_click));
  stand_button->signal_clicked().connect(sigc::mem_fun(*this, &Mainwin::on_stand_click));

  //creates the initial dialog box to enter a player's name and handles all of the things that need it to function properly
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

  while(result)
  {
    result = dialog->run();
    _player_name = e_name.get_text();
    playername_label->set_label(_player_name);
  }

  dialog->close();
  delete dialog;

  current_bet = 0;
}

//destructor for the Mainwin class
Mainwin::~Mainwin()
{}

//method that links the player_game class and the gtk window together and sets the player's name
void Mainwin::setPlayerGame(Player_Game pgame)
{
  _pg = pgame;
  _pg.setName(_player_name);
}

//method that links the player_comm class and gtk window together
void Mainwin::setPlayerComm(player_comm* pcomm)
{
  _pc = pcomm;
  //sets initial conditions for a starting player
  chat_message join = _pg.move_j("join", 0, 0);
  _pc->write(join);
}

//method that sets the label for the gtk window
void Mainwin::setLabel(std::string text)
{
  msg->set_label(text);
}

//method that sets the cards in the player's hand
void Mainwin::setCards(nlohmann::json cards, int num)
{
  std::vector<std::string> f = _pg.setHand(cards, num);
  card_1->set(f[0]);
  card_2->set(f[1]);
  card_3->set(f[2]);
  card_4->set(f[3]);
  card_5->set(f[4]);
}

//method that closes the game when quit is clicked
void Mainwin::on_quit_click()
{
  close();
}

//method that shows the game information when about is clicked
void Mainwin::on_about_click()
{
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

//method that deals with the move stand pat (not exchanging cards)
void Mainwin::on_stand_click()
{
  chat_message info = _pg.move_j("stand", 0, 0);
  _pc->write(info);
}

//method that deals with when check is clicked
void Mainwin::on_check_click()
{
  if(current_bet)
  {
    bet_entry->set_text("Cant Check!!");
    return;
  }
  chat_message info = _pg.move_j("check", 0, 0);
  _pc->write(info);
}

//method that deals with when bet is clicked
void Mainwin::on_bet_click()
{
  int bet_amount;
  chat_message info;

  try
  {
    bet_amount = std::stoi(bet_entry->get_text());
  }
  catch(std::exception e)
  {
    bet_entry->set_text("### Invalid ###");
    return;
  }

  if(bet_amount > _pg.getChipAmount())
  {
    bet_entry->set_text("Not enough money!!");
    return;
  }
  else if(current_bet == 0)
  {
    current_bet = bet_amount;
    info = _pg.move_j("bet", 0, bet_amount);
  }
  else if(bet_amount > current_bet)
  {
    current_bet = bet_amount;
    info = _pg.move_j("raise", 0, bet_amount);
  }
  else if(bet_amount == current_bet)
  {
    info = _pg.move_j("call", 0, bet_amount);
  }
  else
  {
    bet_entry->set_text("Too low!!");
    return;
  }

  _pc->write(info);
}

//method that deals with when fold is clicked
void Mainwin::on_fold_click()
{
  chat_message info = _pg.move_j("fold", 0, 0);
  _pc->write(info);

  card_1->clear();
  card_2->clear();
  card_3->clear();
  card_4->clear();
  card_5->clear();

  exchange_button->set_sensitive(false);
  fold_button->set_sensitive(false);
  check_button->set_sensitive(false);
  bet_button->set_sensitive(false);
  ante_button->set_sensitive(false);
}


//method that deals with when ante is clicked
void Mainwin::on_ante_click()
{
  int ante_amount;
  try
  {
    ante_amount = std::stoi(bet_entry->get_text());
  }
  catch(std::exception e)
  {
    bet_entry->set_text("### Invalid ###");
    return;
  }

  chat_message info = _pg.move_j("ante", 0, ante_amount);
  _pc->write(info);
}

//method that deals with when exchange is clicked
void Mainwin::on_exchange_click()
{
  //creates a gtk dialog box to see the number of cards that are being exchanged and deals with all of the things that need it to function properly
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

  std::vector<int> cards;

  while(result)
  {
    result = dialog->run();
    exchange_cards = e_cards.get_text();
    std::stringstream ss(exchange_cards);
    int num;
    while(ss >> num)
    {
      if(num < 1 || num > 5)
      {
        e_cards.set_text("###INVALID###");
        result = 1;
      }
      else
      {
        cards.push_back(num);
      }
    }
  }
  dialog->close();
  delete dialog;

  //gets the correct cards and sends then to player_comm
  for(auto num : cards)
  { 
    if(num == 1)
      card_1->clear();
    if(num == 2)
      card_2->clear();
    if(num == 3)
      card_3->clear();
    if(num == 4)
      card_4->clear();
    if(num == 5)
      card_5->clear();
  }

  chat_message info = _pg.exchange_j("request_cards", cards.size(), cards);
  _pc->write(info);
}
