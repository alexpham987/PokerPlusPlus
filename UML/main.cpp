#include <iostream>
#include "json.hpp"
#include "Mainwin.h"
#include <gtkmm.h>

int main(int argc, char *argv[]) {
	Gtk::Main kit(argc, argv);
	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("PokerPP.glade");

	Mainwin *win = 0;
	builder->get_widget_derived("Mainwin", win);

	kit.run(*win);
	return 0;
}

