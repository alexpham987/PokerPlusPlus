#include "Mainwin.h"
#include <dirent.h>
#include <fstream>
#include <experimental/filesystem>

Mainwin::Mainwin() {

	set_default_size(500, 200);
    set_title("Java and Donut Express (JADE)");

    Gtk::Box *vbox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));
    add(*vbox);

    Gtk::MenuBar *menubar = Gtk::manage(new Gtk::MenuBar());
    vbox->pack_start(*menubar, Gtk::PACK_SHRINK, 0);
	
	Gtk::MenuItem *menuitem_file = Gtk::manage(new Gtk::MenuItem("_File", true));
    menubar->append(*menuitem_file);
    Gtk::Menu *filemenu = Gtk::manage(new Gtk::Menu());
    menuitem_file->set_submenu(*filemenu);
	
	Gtk::MenuItem *menuitem_save = Gtk::manage(new Gtk::MenuItem("_Save", true));
    menuitem_save->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_save_click));
    filemenu->append(*menuitem_save);
	
	Gtk::MenuItem *menuitem_load = Gtk::manage(new Gtk::MenuItem("_Open", true));
    menuitem_load->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_load_click));
    filemenu->append(*menuitem_load);

    Gtk::MenuItem *menuitem_quit = Gtk::manage(new Gtk::MenuItem("_Quit", true));
    menuitem_quit->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_quit_click));
    filemenu->append(*menuitem_quit);
	
	Gtk::MenuItem *menuitem_view = Gtk::manage(new Gtk::MenuItem("_View", true));
    menubar->append(*menuitem_view);
    Gtk::Menu *viewmenu = Gtk::manage(new Gtk::Menu());
    menuitem_view->set_submenu(*viewmenu);

    Gtk::MenuItem *menuitem_all_products = Gtk::manage(new Gtk::MenuItem("_All Products", true));
    menuitem_all_products->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_view_all_click));
    viewmenu->append(*menuitem_all_products);
	
	Gtk::MenuItem *menuitem_all_customers = Gtk::manage(new Gtk::MenuItem("_All Customers", true));
    menuitem_all_customers->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_list_customers_click));
    viewmenu->append(*menuitem_all_customers);

	
	Gtk::MenuItem *menuitem_create = Gtk::manage(new Gtk::MenuItem("_Create", true));
    menubar->append(*menuitem_create);
    Gtk::Menu *createmenu = Gtk::manage(new Gtk::Menu());
    menuitem_create->set_submenu(*createmenu);

    menuitem_new_coffee = Gtk::manage(new Gtk::MenuItem("_Coffee", true));
    menuitem_new_coffee->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_create_coffee_click));
    createmenu->append(*menuitem_new_coffee);
	
	menuitem_new_donut = Gtk::manage(new Gtk::MenuItem("_Donut", true));
    menuitem_new_donut->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_create_donut_click));
    createmenu->append(*menuitem_new_donut);
	
	Gtk::MenuItem *menuitem_new_customer = Gtk::manage(new Gtk::MenuItem("_Customer", true));
    menuitem_new_customer->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_new_customer_click));
    createmenu->append(*menuitem_new_customer);
	
	Gtk::MenuItem *menuitem_new_order = Gtk::manage(new Gtk::MenuItem("_Order", true));
    menuitem_new_order->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_create_order_click));
    createmenu->append(*menuitem_new_order);
	
	Gtk::MenuItem *menuitem_help = Gtk::manage(new Gtk::MenuItem("_Help", true));
    menubar->append(*menuitem_help);
    Gtk::Menu *helpmenu = Gtk::manage(new Gtk::Menu());
    menuitem_help->set_submenu(*helpmenu);
	
	Gtk::MenuItem *menuitem_about = Gtk::manage(new Gtk::MenuItem("_About", true));
    menuitem_about->signal_activate().connect(sigc::mem_fun(*this, &Mainwin::on_about_click));
    helpmenu->append(*menuitem_about);
	
	
	
	Gtk::Toolbar *toolbar = Gtk::manage(new Gtk::Toolbar);
    vbox->add(*toolbar);

	Gtk::Image *add_java_image = Gtk::manage(new Gtk::Image{"JavaButton.png"});
    Gtk::ToolButton *add_java_button = Gtk::manage(new Gtk::ToolButton(*add_java_image));
    add_java_button->set_tooltip_markup("Create Java");
    add_java_button->signal_clicked().connect(sigc::mem_fun(*this, &Mainwin::on_create_coffee_click));
    toolbar->append(*add_java_button);
	
	Gtk::Image *add_donut_image = Gtk::manage(new Gtk::Image{"DonutButton.png"});
    Gtk::ToolButton *add_donut_button = Gtk::manage(new Gtk::ToolButton(*add_donut_image));
    add_donut_button->set_tooltip_markup("Create Donut");
    add_donut_button->signal_clicked().connect(sigc::mem_fun(*this, &Mainwin::on_create_donut_click));
    toolbar->append(*add_donut_button);
	
	Gtk::Image *list_products_image = Gtk::manage(new Gtk::Image{"ListProducts.png"});
	Gtk::ToolButton *list_products_button = Gtk::manage(new Gtk::ToolButton(*list_products_image));
    list_products_button->set_tooltip_markup("List all Products");
    list_products_button->signal_clicked().connect(sigc::mem_fun(*this, &Mainwin::on_view_all_click));
    toolbar->append(*list_products_button);
	
	Gtk::Image *add_customer_image = Gtk::manage(new Gtk::Image{"CustomerButton.png"});
	Gtk::ToolButton *add_customer_button = Gtk::manage(new Gtk::ToolButton(*add_customer_image));
    add_customer_button->set_tooltip_markup("Add Customer");
    add_customer_button->signal_clicked().connect(sigc::mem_fun(*this, &Mainwin::on_new_customer_click));
    toolbar->append(*add_customer_button);
	
	Gtk::Image *list_customers_image = Gtk::manage(new Gtk::Image{"ListCustomers.png"});
	Gtk::ToolButton *list_customers_button = Gtk::manage(new Gtk::ToolButton(*list_customers_image));
    list_customers_button->set_tooltip_markup("List Customers");
    list_customers_button->signal_clicked().connect(sigc::mem_fun(*this, &Mainwin::on_list_customers_click));
    toolbar->append(*list_customers_button);
	
    vbox->show_all();
}

Mainwin::~Mainwin() {}

void Mainwin::on_quit_click() {
	close();
}

void Mainwin::on_view_all_click() {
	std::ostringstream oss;
	oss	<< _store;
	std::string store = oss.str();
	Gtk::MessageDialog *dialog = new Gtk::MessageDialog("Products");
	dialog->set_transient_for(*this);
	dialog->set_secondary_text(store, true);
	dialog->run();

	dialog->close();
	while(Gtk::Main::events_pending()) Gtk::Main::iteration();

	delete dialog;
	
}

void Mainwin::on_create_donut_click() {
	std::string name;
	double price;
	double cost;
	Frosting fr;
	Filling fi;

	Gtk::Dialog *dialog = new Gtk::Dialog();
	dialog->set_transient_for(*this);
	dialog->set_title("Create a Donut");

	// Name
	Gtk::HBox b_name;

	Gtk::Label l_name{"Name: "};
	l_name.set_width_chars(15);
	b_name.pack_start(l_name, Gtk::PACK_SHRINK);

	Gtk::Entry e_name;
	e_name.set_max_length(50);
	b_name.pack_start(e_name, Gtk::PACK_SHRINK);
	dialog->get_vbox()->pack_start(b_name, Gtk::PACK_SHRINK);

// num
	Gtk::HBox b_num;

	Gtk::Label l_num{"Price: "};
	l_num.set_width_chars(15);
	b_num.pack_start(l_num, Gtk::PACK_SHRINK);

	Gtk::Entry e_num;
	e_num.set_max_length(50);
	b_num.pack_start(e_num, Gtk::PACK_SHRINK);
	dialog->get_vbox()->pack_start(b_num, Gtk::PACK_SHRINK);
	
	Gtk::HBox b_cost;

	Gtk::Label l_cost{"Cost: "};
	l_cost.set_width_chars(15);
	b_cost.pack_start(l_cost, Gtk::PACK_SHRINK);

	Gtk::Entry e_cost;
	e_cost.set_max_length(50);
	b_cost.pack_start(e_cost, Gtk::PACK_SHRINK);
	dialog->get_vbox()->pack_start(b_cost, Gtk::PACK_SHRINK);
	
	Gtk::HBox b_frost; 
	
	Gtk::Label l_frost{"Frosting: "};
	l_frost.set_width_chars(15);
	b_frost.pack_start(l_frost, Gtk::PACK_SHRINK);
	
	Gtk::ComboBoxText c_frost;
	c_frost.set_size_request(160);
	c_frost.append("None");
	c_frost.append("Chocolate");
	c_frost.append("Vanilla");
	c_frost.append("Pink");
	b_frost.pack_start(c_frost, Gtk::PACK_SHRINK);
	dialog->get_vbox()->pack_start(b_frost, Gtk::PACK_SHRINK);
	
	Gtk::HBox b_fill;
	
	Gtk::Label l_fill{"Filling: "};
	l_fill.set_width_chars(15);
	b_fill.pack_start(l_fill, Gtk::PACK_SHRINK);
	
	Gtk::ComboBoxText c_fill;
	c_fill.set_size_request(160);
	c_fill.append("None");
	c_fill.append("Creme");
	c_fill.append("Bavarian");
	c_fill.append("Strawberry");
	b_fill.pack_start(c_fill, Gtk::PACK_SHRINK);
	dialog->get_vbox()->pack_start(b_fill, Gtk::PACK_SHRINK);
	
		
	Gtk::CheckButton m_sprink{"Sprinkles"};
	dialog->get_vbox()->pack_start(m_sprink, Gtk::PACK_SHRINK);

	// Show dialog
	dialog->add_button("Cancel", 0);
	dialog->add_button("Create", 1);
	dialog->show_all();
	int result; // of the dialog (1 = OK)
	bool fail = true;  // set to true if any data is invalid

	while (fail) {
		fail = false;  // optimist!
		result = dialog->run();
		if (result != 1) {delete dialog; return;}
		try {
			price = std::stod(e_num.get_text());
		} catch(std::exception e) {
			e_num.set_text("### Invalid ###");
			fail = true;
		}
		try {
			cost = std::stod(e_cost.get_text());
		} catch(std::exception e) {
			e_cost.set_text("### Invalid ###");
			fail = true;
		}
		name = e_name.get_text();
		if (name.size() == 0) {
			e_name.set_text("### Invalid ###");
			fail = true;
		}
	}
	
	name = e_name.get_text();
	price = std::stod(e_num.get_text());
	cost = std::stod(e_cost.get_text());
	fi = static_cast<Filling>(c_fill.get_active_row_number());
	fr = static_cast<Frosting>(c_frost.get_active_row_number());
	bool s = m_sprink.get_active();
	_store.add_product(new Donut{name, price, cost, fr, s, fi});
	
	delete dialog;
}       

void Mainwin::on_create_coffee_click() {
	
	std::string name;
	double price;
	double cost;
	Roast darkness;
	Shot shot;
	int run = 1;

	Gtk::Dialog *dialog = new Gtk::Dialog();
	dialog->set_transient_for(*this);
	dialog->set_title("Create Java");

	// Name
	Gtk::HBox b_name;

	Gtk::Label l_name{"Name: "};
	l_name.set_width_chars(15);
	b_name.pack_start(l_name, Gtk::PACK_SHRINK);

	Gtk::Entry e_name;
	e_name.set_max_length(50);
	b_name.pack_start(e_name, Gtk::PACK_SHRINK);
	dialog->get_vbox()->pack_start(b_name, Gtk::PACK_SHRINK);

// num
	Gtk::HBox b_num;

	Gtk::Label l_num{"Price: "};
	l_num.set_width_chars(15);
	b_num.pack_start(l_num, Gtk::PACK_SHRINK);

	Gtk::Entry e_num;
	e_num.set_max_length(50);
	b_num.pack_start(e_num, Gtk::PACK_SHRINK);
	dialog->get_vbox()->pack_start(b_num, Gtk::PACK_SHRINK);
	
	Gtk::HBox b_cost;

	Gtk::Label l_cost{"Cost: "};
	l_cost.set_width_chars(15);
	b_cost.pack_start(l_cost, Gtk::PACK_SHRINK);

	Gtk::Entry e_cost;
	e_cost.set_max_length(50);
	b_cost.pack_start(e_cost, Gtk::PACK_SHRINK);
	dialog->get_vbox()->pack_start(b_cost, Gtk::PACK_SHRINK);
	
	Gtk::HBox b_roast; 
	
	Gtk::Label l_roast{"Roast "};
	l_roast.set_width_chars(15);
	b_roast.pack_start(l_roast, Gtk::PACK_SHRINK);
	
	Gtk::ComboBoxText c_roast;
	c_roast.set_size_request(160);
	c_roast.append("Blonde");
	c_roast.append("Light");
	c_roast.append("Medium");
	c_roast.append("Dark");
	c_roast.append("Very Dark");
	b_roast.pack_start(c_roast, Gtk::PACK_SHRINK);
	dialog->get_vbox()->pack_start(b_roast, Gtk::PACK_SHRINK);
	
	// Show dialog
	dialog->add_button("Cancel", 0);
	dialog->add_button("Create", 1);
	dialog->add_button("Add Shot", 2);
	dialog->show_all();
	int result;
	bool fail = true;  // set to true if any data is invalid

	while (fail) {
		fail = false;  // optimist!
		result = dialog->run();
		if (result != 1 && result != 2) {delete dialog; return;}

		try {
			price = std::stod(e_num.get_text());
		} catch(std::exception e) {
			e_num.set_text("### Invalid ###");
			fail = true;
		}
		try {
			cost = std::stod(e_cost.get_text());
		} catch(std::exception e) {
			e_cost.set_text("### Invalid ###");
			fail = true;
		}
		name = e_name.get_text();
		if (name.size() == 0) {
			e_name.set_text("### Invalid ###");
			fail = true;
		}
	}
	
	price = std::stod(e_num.get_text());
	cost = std::stod(e_cost.get_text());
	darkness = static_cast<Roast>(c_roast.get_active_row_number());
	
	Java* java = new Java{name, price, cost, darkness};
	_store.add_product(java);
	
	
	if(result == 2) {
		while(run) {
			
			Gtk::Dialog *dialog = new Gtk::Dialog();
			dialog->set_transient_for(*this);
			dialog->set_title("Create Java");

	// Name
			Gtk::HBox b_shots;

			Gtk::Label l_shots{"Flavors: "};
			l_shots.set_width_chars(15);
			b_shots.pack_start(l_shots, Gtk::PACK_SHRINK);
		
			Gtk::ComboBoxText c_shots;
			c_shots.set_size_request(160);
			c_shots.append("None");
			c_shots.append("Chocolate");
			c_shots.append("Vanilla");
			c_shots.append("Peppermint");
			c_shots.append("Hazelnut");
			b_shots.pack_start(c_shots, Gtk::PACK_SHRINK);
			dialog->get_vbox()->pack_start(b_shots, Gtk::PACK_SHRINK);
			
			dialog->add_button("Cancel", 0);
			dialog->add_button("Create", 1);
			dialog->add_button("Add", 2);
			dialog->show_all();
			int result = dialog->run();
			
			dialog->close();
			while (Gtk::Main::events_pending())  Gtk::Main::iteration();
			
			if(result == 2) {
				if(c_shots.get_active_row_number() == 0) continue;
				shot = static_cast<Shot>(c_shots.get_active_row_number());
				java->add_shot(shot);
			}
			
			delete dialog;
			
			if(result != 2) run = 0;
		}
	}
	
	delete dialog;
}

void Mainwin::on_create_order_click() {
	int quantity;
	int index;
	int run = 1;
	Order* order = new Order;
	
	
	while(run)
	{
		
		Gtk::Dialog *dialog = new Gtk::Dialog();
		dialog->set_transient_for(*this);
		dialog->set_title("Add Order");
		
		std::ostringstream oss;
		oss << _store;
		Gtk::Label *label = new Gtk::Label(oss.str());
		label->set_use_markup(true);
		dialog->get_content_area()->pack_start(*label);
		label->show();

	// Name
		Gtk::HBox b_name;

		Gtk::Label l_name{"Product Index: "};
		l_name.set_width_chars(15);
		b_name.pack_start(l_name, Gtk::PACK_SHRINK);

		Gtk::Entry e_name;
		e_name.set_max_length(50);
		b_name.pack_start(e_name, Gtk::PACK_SHRINK);
		dialog->get_vbox()->pack_start(b_name, Gtk::PACK_SHRINK);

   // num
		Gtk::HBox b_num;

		Gtk::Label l_num{"Quantity: "};
		l_num.set_width_chars(15);
		b_num.pack_start(l_num, Gtk::PACK_SHRINK);

		Gtk::Entry e_num;
		e_num.set_max_length(50);
		b_num.pack_start(e_num, Gtk::PACK_SHRINK);
		dialog->get_vbox()->pack_start(b_num, Gtk::PACK_SHRINK);

	// Show dialog
		dialog->add_button("Cancel", 0);
		dialog->add_button("OK", 1);
		dialog->add_button("Done", 2);
		dialog->show_all();
		int result = dialog->run();

		dialog->close();
		while (Gtk::Main::events_pending())  Gtk::Main::iteration();
		
		if (result == 1) {
			index = std::stod(e_name.get_text());
			quantity = std::stod(e_num.get_text());
			try {
				order->add_product(_store.get_product(index-1), quantity);
			}
			catch(std::out_of_range& e) {
				std::cerr << e.what() << std::endl;
			}
			
		}
		else if(result == 2) {run = 0;}
		
		else { return; }
		
		delete dialog;
		delete label;
	}
	
	Gtk::Dialog *dialog = new Gtk::Dialog();
	dialog->set_transient_for(*this);
	dialog->set_title("Which customer are you?");
	
	std::ostringstream oss;
	oss << _store.customer_to_string(0);
	Gtk::Label *label = new Gtk::Label(oss.str());
	label->set_use_markup(true);
	dialog->get_content_area()->pack_start(*label);
	label->show();

	Gtk::HBox b_index;

	Gtk::Label l_index{"Customer Index: "};
	l_index.set_width_chars(15);
	b_index.pack_start(l_index, Gtk::PACK_SHRINK);

	Gtk::Entry e_index;
	e_index.set_max_length(50);
	b_index.pack_start(e_index, Gtk::PACK_SHRINK);
	dialog->get_vbox()->pack_start(b_index, Gtk::PACK_SHRINK);
	
	dialog->add_button("OK", 1);
	dialog->show_all();
	int result = dialog->run();
	
	dialog->close();
	while (Gtk::Main::events_pending())  Gtk::Main::iteration();
	
	int customer = std::stod(e_index.get_text()) - 1;
	_store.add_order(order, customer);
	
	delete dialog;
	delete label;
	
	Gtk::Dialog *dialog2 = new Gtk::Dialog();
	dialog2->set_transient_for(*this);
	dialog2->set_title("Order Summary");
	
	std::string order_str = _store.order_to_string(customer);
	Gtk::Label *label2 = new Gtk::Label(order_str);
	label2->set_use_markup(true);
	dialog2->get_content_area()->pack_start(*label2);
	label2->show();
	
	dialog2->add_button("OK", 1);
	dialog2->show_all();
	int result2 = dialog2->run();
	
	dialog2->close();
	while (Gtk::Main::events_pending())  Gtk::Main::iteration();
	
	delete dialog2;
	delete label2;
	
	
	//_store.add_order(order, customer);
}
	
	


void Mainwin::on_list_customers_click() {
	std::string customers = _store.customer_to_string(0);
	Gtk::MessageDialog *dialog = new Gtk::MessageDialog("Customers");
	dialog->set_transient_for(*this);
	dialog->set_secondary_text(customers, true);
	dialog->run();

	dialog->close();
	while(Gtk::Main::events_pending()) Gtk::Main::iteration();

	delete dialog;
}

void Mainwin::on_new_customer_click() {

	std::string name;
	std::string phone;

	Gtk::Dialog *dialog = new Gtk::Dialog();
	dialog->set_transient_for(*this);
	dialog->set_title("Add Customer");

	// Name
	Gtk::HBox b_name;

	Gtk::Label l_name{"Name: "};
	l_name.set_width_chars(15);
	b_name.pack_start(l_name, Gtk::PACK_SHRINK);

	Gtk::Entry e_name;
	e_name.set_max_length(50);
	b_name.pack_start(e_name, Gtk::PACK_SHRINK);
	dialog->get_vbox()->pack_start(b_name, Gtk::PACK_SHRINK);

// num
	Gtk::HBox b_num;

	Gtk::Label l_num{"Phone: "};
	l_num.set_width_chars(15);
	b_num.pack_start(l_num, Gtk::PACK_SHRINK);

	Gtk::Entry e_num;
	e_num.set_max_length(50);
	b_num.pack_start(e_num, Gtk::PACK_SHRINK);
	dialog->get_vbox()->pack_start(b_num, Gtk::PACK_SHRINK);

	// Show dialog
	dialog->add_button("Cancel", 0);
	dialog->add_button("Create", 1);
	dialog->show_all();
	int result = dialog->run();

	dialog->close();
	while (Gtk::Main::events_pending())  Gtk::Main::iteration();


	if (result) {
		name = e_name.get_text();
		phone = e_num.get_text();
		try {
			_store.add_customer(new Customer{name, phone});
		}	
		catch(std::exception& e) {
			std::string msg = "Please input valid information.";        
			Gtk::MessageDialog dialog(*this , "Error Creating Customer", true, Gtk::MessageType::MESSAGE_ERROR);        
			dialog.set_secondary_text(msg, true);        
			dialog.run();
		}
		
	}
		
delete dialog;

}

void Mainwin::on_about_click() { 
	Gtk::AboutDialog dialog{};    
	dialog.set_transient_for(*this);    
	dialog.set_program_name("JADE");    
	dialog.set_version("Version 1.3.0");    
	dialog.set_copyright("Copyright 2019");    
	dialog.set_license_type(Gtk::License::LICENSE_GPL_3_0);    
	std::vector< Glib::ustring > authors = {"Bailey A. Brown"};    
	dialog.set_authors(authors);    
	std::vector< Glib::ustring > artists = {"Bailey A. Brown"}; // vector redacted for space    
	dialog.set_artists(artists);
	dialog.run(); 
	}


void Mainwin::on_save_click() { 

	std::string _filename;

	try {
   
		Gtk::FileChooserDialog dialog(*this, "Please choose a file",
			  Gtk::FileChooserAction::FILE_CHOOSER_ACTION_SAVE);
		auto filter_dat = Gtk::FileFilter::create();
		filter_dat->set_name("DAT files");
		filter_dat->add_pattern("*.dat");
		dialog.add_filter(filter_dat);

		auto filter_any = Gtk::FileFilter::create();
		filter_any->set_name("Any files");
		filter_any->add_pattern("*");
		dialog.add_filter(filter_any);

		dialog.set_filename("untitled.dat");

		dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
		dialog.add_button("_Save", Gtk::RESPONSE_OK);

		int result = dialog.run();

		if (result != Gtk::RESPONSE_OK) return;
		_filename = dialog.get_filename();

		std::ofstream ofs{_filename};
		
		if (ofs) _store.save(ofs);
		else std::cerr << "Unable to open for save: " << _filename << std::endl;
		
	} catch (std::exception e) {
		std::cerr << "Save aborted" << std::endl;
	}

}

void Mainwin::on_load_click() { 
	
	std::string _filename;

	try {
	
		Gtk::FileChooserDialog dialog(*this, "Please choose a file",
			Gtk::FileChooserAction::FILE_CHOOSER_ACTION_OPEN);

		auto filter_dat = Gtk::FileFilter::create();
		filter_dat->set_name("DAT files");
		filter_dat->add_pattern("*.dat");
		dialog.add_filter(filter_dat);

		auto filter_text = Gtk::FileFilter::create();
		filter_text->set_name("Text files");
		filter_text->add_mime_type("text/plain");
		dialog.add_filter(filter_text);

		auto filter_any = Gtk::FileFilter::create();
		filter_any->set_name("Any files");
		filter_any->add_pattern("*");
		dialog.add_filter(filter_any);

		dialog.set_filename("untitled.dat");

		dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
		dialog.add_button("_Open", Gtk::RESPONSE_OK);

		int result = dialog.run();

		if (result != Gtk::RESPONSE_OK) return;
		_filename = dialog.get_filename();
	
		std::ifstream ifs{_filename};
		if (ifs) _store = Store{ifs};
		else std::cerr << "Unable to open for load: " << _filename << std::endl;
	} catch(std::exception e) {
		std::cerr << "Load aborted" << std::endl;
	}
	
}
