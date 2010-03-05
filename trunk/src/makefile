CC=gcc
CFLAGS=-g -Wall -O3 `sdl-config --cflags`

#-----------------------------------------------------------------------
# Build cu-cam
LDFLAGS=`sdl-config --libs` -lSDL_ttf
SOURCES=TinyCurses.c test-view.c
OBJECTS=$(SOURCES:.c=.o)

EXECUTABLE=test-view

all: $(EXECUTABLE)
	

test-view: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

# Implicit rule to compile all .c to .o
.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

# Cleanup rules

clean:
	rm -fv *.o $(EXECUTABLE)  *~
