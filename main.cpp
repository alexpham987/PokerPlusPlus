#include "card.h"
#include "chip.h"
#include <iostream>

int main() {
	Card c(2,C);
	std::cout << c.card_to_filename() << std::endl;

	Chip c1(GREEN);
	std::cout << c1.chip_to_filename() << std::endl;

	return 0;
}
