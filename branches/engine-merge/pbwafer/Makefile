CC=gcc
CFLAGS=-g -Wall -O0 `sdl-config --cflags`

LDFLAGS=`sdl-config --libs` -lSDL_ttf

################################################################################
# Build tests
################################################################################
T_EXECUTABLE=test-vidsdl test-input test-winsys

test: $(T_EXECUTABLE)
	

T_VIDSDL_SOURCES=vid_sdl.c test-vidsdl.c
T_VIDSDL_OBJECTS=$(T_VIDSDL_SOURCES:.c=.o)

test-vidsdl: $(T_VIDSDL_OBJECTS)
	$(CC) $(LDFLAGS) $(T_VIDSDL_OBJECTS) -o $@

T_INPUT_SOURCES=input.c test-input.c
T_INPUT_OBJECTS=$(T_INPUT_SOURCES:.c=.o)

test-input: $(T_INPUT_OBJECTS)
	$(CC) $(LDFLAGS) $(T_INPUT_OBJECTS) -o $@

T_WINSYS_SOURCES=vid_sdl.c view.c render_sdl.c input.c cmd.c utils.c list.c menu.c console.c buf.c test-winsys.c
T_WINSYS_OBJECTS=$(T_WINSYS_SOURCES:.c=.o)

test-winsys: $(T_WINSYS_OBJECTS)
	$(CC) -g $(LDFLAGS) $(T_WINSYS_OBJECTS) -o $@

################################################################################
# Implicit rule to compile all .c to .o
################################################################################
.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

################################################################################
# Cleanup rules
################################################################################
clean:
	rm -fv *.o object-gen/*.o $(EXECUTABLE) $(T_EXECUTABLE) *~

clean-test:
	rm -fv *.o $(T_EXECUTABLE)  *~
