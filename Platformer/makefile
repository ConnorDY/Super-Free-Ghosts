CXX=g++
LIBS=-lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
LDFLAGS=-Wall -Wextra -pedantic -g3
CPPFLAGS=-Wall -Wextra -pedantic
CXXFLAGS=-std=c++11 -g3
OBJS=state_manager.o texture_manager.o sound_manager.o input_handler.o object.o projectile.o player.o block.o state.o menu_state.o level01_state.o main.o
OUTPUT=platformer

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

clean:
	rm -f $(OBJS) $(OUTPUT)
