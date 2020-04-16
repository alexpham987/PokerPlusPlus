# this is an example makefile


all: asio-1.12.2 src/main_player src/main_dealer \
	 src/player_comm src/player_game src/dealer_comm src/dealer_game \
     src/card src/chip src/deck src/hand src/stack src/json src/Mainwin \
     src/pot src/round \

asio-1.12.2:
	tar xzf asio-1.12.2.tar.gz


# many examples on the web show the 'pkg-config' commands
# being executed during the make process.  this just
# does it once when the makefile is initially parsed
GTKLINKFLAGS=$(shell pkg-config --libs gtkmm-3.0)
GTKCOMPILEFLAGS=$(shell pkg-config --cflags gtkmm-3.0)


CXXFLAGS+= -DASIO_STANDALONE -Wall -O0 -g -std=c++11
CPPFLAGS+=-I./include -I./asio-1.12.2/include -I./src ${GTKCOMPILEFLAGS}
LDLIBS+= -lpthread ${GTKLINKFLAGS}

src/shuffle_test:src/shuffle_test.cpp src/cards.cpp
	${CXX} -o $@ $^ -Wall -std=c++11

src/char_count_test:src/char_count_test.cpp src/char_count.c
	${CXX} -o $@ $^ -Wall -std=c++11

test:
	./src/shuffle_test

clean:
	-rm -rf asio-1.12.2
	-rm -f src/player_main
	-rm -f src/dealer_main		
	-rm -f src/player_comm
	-rm -f src/player_game
	-rm -f src/dealer_comm
	-rm -f src/dealer_game
	-rm -f src/card
	-rm -f src/deck
	-rm -f src/hand
	-rm -f src/chip
	-rm -f src/stack
	-rm -f src/pot
	-rm -f src/round
	-rm -f src/json
	-rm -f src/Mainwin
