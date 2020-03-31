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
        Mainwin();
        virtual ~Mainwin();
    protected:
        void on_quit_click();
	void on_check_click();
	void on_bet_click();
	void on_raise_click();
	void on_fold_click();
	void on_about_click();

    private:
        Gtk::Label* msg;

};
#endif
