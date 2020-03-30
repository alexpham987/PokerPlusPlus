#ifndef __MAINWIN_H
#define __MAINWIN_H

// include other classes
#include <gtkmm.h>
#include <string>
#include <exception>
#include <iostream>
#include <sstream>

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
	void on_about_click();
		
    private:                                               
        Gtk::Label* msg;
	Gtk::Button* fold_button;
	Gtk::Button* check;
	Gtk::Button* bet;
	Gtk::Entry* bet_entry;
	Glib::RefPtr<Gtk::Builder> builder;

};
#endif 
