#ifndef __CHIP_H
#define __CHIP_H

#include <string>
#include <vector>
#include <stdexcept>

enum Color {WHITE ,RED, BLUE};
const std::vector<std::string> color_to_string = {"White", "Red", "Blue"};

const int MAX_COLOR = 3;


class Chip {

  public:
	Chip(Color color);
	Color color() const;
	int value() const;
	
  private:
	Color _color;
	int _value;

};

#endif