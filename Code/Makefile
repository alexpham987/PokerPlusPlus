#compiler options
CXXFLAGS+= -DASIO_STANDALONE -Wall -std=c++11 -I./include -I./asio-1.12.2/include -g

#source files
SOURCES=$(wildcard *.cpp)

#object files
OBJECTS=$(SOURCES:.cpp=.o)

#main link objects
MOBJECTSP=$(filter-out main_dealer.o,$(OBJECTS))
MOBJECTSD=$(filter-out main_player.o,$(OBJECTS))

#test link objects
TOBJECTS=$(filter-out main.o,$(OBJECTS))

#included libraries
INCLUDE=`/usr/bin/pkg-config gtkmm-3.0 --cflags --libs` 

#executable filename
EXECUTABLEP=pokerplayer
EXECUTABLED=pokerdealer

all: $(EXECUTABLEP) $(EXECUTABLED)

#Special symbols used:
#$^ - is all the dependencies (in this case =$(OBJECTS) )
#$@ - is the result name (in this case =$(EXECUTABLE) )

$(EXECUTABLEP): $(MOBJECTSP)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(INCLUDE) 

#$(EXECUTABLED): $(MOBJECTSD)
#	$(CXX) $(CXXFLAGS) $^ -o $@ $(INCLUDE)


$(EXECUTABLED):dealer_game.cpp round.cpp chip.cpp hand.cpp deck.cpp \
               stack.cpp main_dealer.cpp  \
               pot.cpp card.cpp dealer_comm.cpp
	$(CXX) $(CXXFLAGS) -DSERVER $^ -o $@ -lpthread

test: CXXFLAGS+= -g
test: $(TOBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(INCLUDE)

debug: CXXFLAGS+= -g
debug: $(EXECUTABLE)

%.o: %.cpp *.h
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

clean:
	-rm -f $(EXECUTABLEP) test $(OBJECTS) $(EXECUTABLED)


