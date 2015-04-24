CXX=g++
LIBS=-lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
LDFLAGS=-Wall -Wextra -pedantic
CPPFLAGS=-Wall -Wextra -pedantic -I.
CXXFLAGS=-std=c++14 -g3
SRCS=$(wildcard *.cpp) $(wildcard animations/*.cpp)
OBJS=$(SRCS:.cpp=.o)
OUTPUT=platformer

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

%.o: %.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $< -MMD -MQ $@

clean:
	rm -f $(OBJS) $(OUTPUT) $(OBJS:.o=.d)

-include *.d
