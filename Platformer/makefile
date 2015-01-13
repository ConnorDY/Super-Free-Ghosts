CXX=g++
LIBS=-lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
CPPFLAGS=-Wall
CXXFLAGS=-std=c++11
OBJS=texture_manager.o sound_manager.o object.o projectile.o player.o block.o state.o level01_state.o main.o
OUTPUT=platformer

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

clean:
	rm -f $(OBJS) $(OUTPUT)
