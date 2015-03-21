CXX=g++
LIBS=-lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
LDFLAGS=-Wall -Wextra -pedantic
CPPFLAGS=-Wall -Wextra -pedantic
CXXFLAGS=-std=c++14 -g3
OBJS=state_manager.o texture_manager.o sound_manager.o input_handler.o \
     object.o projectile.o player.o zombie.o obelisk.o room.o block.o \
     state.o menu_state.o options_state.o level01_state.o main.o
OUTPUT=platformer

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

%.o: %.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $< -MMD -MQ $@

clean:
	rm -f $(OBJS) $(OUTPUT) $(OBJS:.o=.d)

-include *.d
