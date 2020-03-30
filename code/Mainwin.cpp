#include "Mainwin.h"
#include <dirent.h>
#include <fstream>
#include <experimental/filesystem>

Mainwin::Mainwin(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) : Gtk::Window(cobject), builder{refGlade} {

	builder->get_widget("fold_button", fold_button);
	builder->get_widget("check_button", check_button);
	builder->get_widget("bet_button", bet_button);
	builder->get_widget("bet_entry", bet_entry);
	builder->get_widget("msg", msg);
	builder->get_widget("MenuBar", MenuBar);
	builder->get_widget("menuitem_help", menuitem_help);
	builder->get_widget("menuitem_about", menuitem_about);
	builder->get_widget("menuitem_file", menuitem_file);
	builder->get_widget("menuitem_quit", menuitem_quit);

	menuitem_about->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_about_click));

	menuitem_quit->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_quit_click));
	

}

Mainwin::~Mainwin() {}

void Mainwin::on_quit_click() {
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
}

void Mainwin::on_bet_click() {
}

void Mainwin::on_fold_click() {
}
