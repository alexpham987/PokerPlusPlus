#ifndef __CHIP_H
#define __CHIP_H

#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <map>

//enum that holds the 3 colors of chips
enum Color {GREEN ,RED, BLUE};

const int MAX_COLOR = 3;

class Chip
{
  public:
	Chip(Color color);
	Color color() const;
	int value() const;
	std::string chip_to_filename();

  private:
	Color _color; //attribute that represents the color of a chip
	int _value; //attribute that represents the value of a chip
};

#endif
