#ifndef __CHIP_H
#define __CHIP_H

#include <string>
#include <vector>
#include <stdexcept>
#include <gtkmm.h>
#include <sstream>
#include <map>

enum Color {GREEN ,RED, BLUE};

const int MAX_COLOR = 3;


class Chip {

  public:
	Chip(Color color);
	Color color() const;
	int value() const;
	std::string chip_to_filename();
	
  private:
	Color _color;
	int _value;
	Gtk::Image *chipimage;

};

#endif
