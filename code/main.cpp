#include <iostream>
#include "json.hpp"
#include "Mainwin.h"
#include <gtkmm.h>

int main(int argc, char *argv[]) {

   auto app = Gtk::Application::create(argc, argv, "edu.uta.cse3310.POKERPP.v1_0");
   
   Mainwin win;
   
   return app->run(win);

}
