CC=gcc
CFLAGS=-g -Wall -O3 `sdl-config --cflags`

LDFLAGS=`sdl-config --libs` -lSDL_ttf
SOURCES=TinyCurses.c test-view.c
OBJECTS=$(SOURCES:.c=.o)

EXECUTABLE=test-view

all: $(EXECUTABLE)

test-view: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	
# build tests

T_EXECUTABLE=test-move

test: $(T_EXECUTABLE)
	

T_MOVE_LDFLAGS=`sdl-config --libs` -lSDL_ttf
T_MOVE_SOURCES=TinyCurses.c test-move.c map.c
T_MOVE_OBJECTS=$(T_MOVE_SOURCES:.c=.o)

test-move: $(T_MOVE_OBJECTS)
	$(CC) $(T_MOVE_LDFLAGS) $(T_MOVE_OBJECTS) -o $@

T_POLL_SOURCES=TinyCurses.c test-poll.c map.c
T_POLL_OBJECTS=$(T_POLL_SOURCES:.c=.o)

test-poll: $(T_POLL_OBJECTS)
	$(CC) $(T_MOVE_LDFLAGS) $(T_POLL_OBJECTS) -o $@

# Implicit rule to compile all .c to .o
.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

# Cleanup rules

clean:
	rm -fv *.o $(EXECUTABLE)  *~

clean-test:
	rm -fv *.o $(T_EXECUTABLE)  *~
