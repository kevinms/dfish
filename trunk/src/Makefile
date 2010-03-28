CC=gcc
CFLAGS=-g -Wall -O3 `sdl-config --cflags`

LDFLAGS=`sdl-config --libs` -lSDL_ttf
SOURCES=TinyCurses.c test-view.c
OBJECTS=$(SOURCES:.c=.o)

all: $(EXECUTABLE)
	

# build tests
T_EXECUTABLE=test-poll

test: $(T_EXECUTABLE)
	

T_POLL_LDFLAGS=`sdl-config --libs` -lSDL_ttf -lm
T_POLL_SOURCES=TinyCurses.c test-poll.c map.c keys.c system.c object.c utils.c planet.c star.c list.c
T_POLL_OBJECTS=$(T_POLL_SOURCES:.c=.o)

test-poll: $(T_POLL_OBJECTS)
	$(CC) $(T_POLL_LDFLAGS) $(T_POLL_OBJECTS) -o $@

# Implicit rule to compile all .c to .o
.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

# Cleanup rules
clean:
	rm -fv *.o object-gen/*.o $(EXECUTABLE)  *~

clean-test:
	rm -fv *.o $(T_EXECUTABLE)  *~
