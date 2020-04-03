#include "chip.h"

Chip::Chip(Color color) : _color{color}
{
	if(color < 0 || color > MAX_COLOR)
		throw std::runtime_error("Color : Out of Range!");
	
	if(color == GREEN)
		_value = 5;
	else if(color == RED)
		_value = 1;
	else
		_value = 25;

	chipimage = Gtk::manage(new Gtk::Image{this->chip_to_filename()});	
}
	
Color Chip::color() const {return _color;}

int Chip::value() const {return _value;}

std::string Chip::chip_to_filename() {
	std::stringstream ss;

	ss << "Chips/chip_";
	
	std::map<Color, std::string> color_string = 
	{
		{Color::GREEN, "green"},
		{Color::RED, "red"},
		{Color::BLUE, "green"},
	};
	ss << color_string[_color];
	ss << ".png";

	return ss.str();
}


