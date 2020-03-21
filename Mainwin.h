#ifndef __MAINWIN_H
#define __MAINWIN_H

#include <gtkmm.h>
#include <string>
#include <exception>
#include <iostream>
#include <sstream>
#include "store.h"
#include "java.h"
#include "donut.h"
#include "order.h"

class Mainwin : public Gtk::Window
{
    public:
        Mainwin();
        virtual ~Mainwin();
    protected:        
        void on_quit_click(); 
        void on_view_all_click(); 
		void on_create_coffee_click();        
        void on_create_donut_click();
		void on_list_customers_click();
		void on_new_customer_click();
		void on_about_click();
		void on_save_click();
		void on_load_click();
		void on_create_order_click();
		
    private:                   
        Store _store{"JADE"};                             
        Gtk::Label* msg;
		Gtk::MenuItem* menuitem_new_coffee;
		Gtk::MenuItem* menuitem_new_donut;
};
#endif 