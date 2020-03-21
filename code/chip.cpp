#include "chip.h"

Chip::Chip(Color color) : _color{color}
{
	if(color < 0 || color > MAX_COLOR)
		throw std::runtime_error("Color : Out of Range!");
	
	if(color == WHITE)
		_value = 1;
	else if(color == RED)
		_value = 5;
	else
		_value = 25;
	
}
	
Color Chip::color() const {return _color;}

int Chip::value() const {return _value;}