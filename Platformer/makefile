STATIC_RESOURCES=true

CC=gcc
CXX=g++

LIBS=-lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

LDFLAGS=-Wall -Wextra -pedantic -fwhole-program
CPPFLAGS=-Wall -Wextra -pedantic -I.
CXXFLAGS=-std=c++14 -g3 -pipe

TOOL_CFLAGS=-std=c11 -Ofast -march=native -pipe
TOOL_LDFLAGS=-s
TOOL_CPPFLAGS=-Wall -Wextra -pedantic

DIRECTORIES=./ animations/ weapons/
SRCS=$(wildcard $(addsuffix *.cpp,$(DIRECTORIES)))
OBJS=$(SRCS:.cpp=.o)
OUTPUT=platformer

ifeq ($(STATIC_RESOURCES), true)
	OBJS += static_resource.o
	CPPFLAGS += -DSTATIC_RESOURCES
endif

all: $(OUTPUT)

ifeq ($(STATIC_RESOURCES), true)
main.o options_state.o level01_state.o menu_state.o dialogue.o: static_resource.h
endif

$(OUTPUT): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

%.o: %.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $< -MMD -MQ $@

static_resource.h static_resource.s: ../tools/make_res res
	../tools/make_res res

../tools/make_res: ../tools/make_res.o

../tools/%.o: ../tools/%.c
	$(CC) -c $(TOOL_CPPFLAGS) $(TOOL_CFLAGS) -o $@ $< -MMD -MQ $@

clean:
	rm -f $(OBJS) $(OUTPUT) $(OBJS:.o=.d)

-include $(addsuffix *.d,$(DIRECTORIES))
