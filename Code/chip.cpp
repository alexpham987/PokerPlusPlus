#include "chip.h"

//constructor initializes _color with the passed in parameter
Chip::Chip(Color color) : _color{color}
{
  //error checking
  if(color < 0 || color > MAX_COLOR)
    throw std::runtime_error("Color Out of Range!");

  //initializes _value based on the color
  if(color == GREEN)
    _value = 5;
  else if(color == RED)
    _value = 1;
  else
    _value = 25;
}

//method that returns the _color attribute
Color Chip::color() const
{
  return _color;
}

//method that returns the _value attribute
int Chip::value() const
{
  return _value;
}

//method that converts the chip to the correct filename
std::string Chip::chip_to_filename()
{
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


