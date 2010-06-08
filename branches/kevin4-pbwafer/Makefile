CC=gcc
CFLAGS=-g -Wall -O3 `sdl-config --cflags`

LDFLAGS=`sdl-config --libs` -lSDL_ttf
SOURCES=vid_sdl.c
OBJECTS=$(SOURCES:.c=.o)

all: $(EXECUTABLE)
	

################################################################################
# build tests
################################################################################
T_EXECUTABLE=test-vidsdl test-input test-winsys test-net-client test-net-server

test: $(T_EXECUTABLE)
	

T_VIDSDL_SOURCES=vid_sdl.c test-vidsdl.c
T_VIDSDL_OBJECTS=$(T_VIDSDL_SOURCES:.c=.o)

test-vidsdl: $(T_VIDSDL_OBJECTS)
	$(CC) $(LDFLAGS) $(T_VIDSDL_OBJECTS) -o $@

T_INPUT_SOURCES=input.c test-input.c
T_INPUT_OBJECTS=$(T_INPUT_SOURCES:.c=.o)

test-input: $(T_INPUT_OBJECTS)
	$(CC) $(LDFLAGS) $(T_INPUT_OBJECTS) -o $@

T_NET_SOURCES=buf.c net.c cmd.c input.c proto.c list.c vid_sdl.c utils.c
T_NET_OBJECTS=$(T_NET_SOURCES:.c=.o)

test-net-client: $(T_NET_OBJECTS) test-net-client.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(T_NET_OBJECTS) test-net-client.c -o $@
test-net-server: $(T_NET_OBJECTS) test-net-server.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(T_NET_OBJECTS) test-net-server.c -o $@
test-net: test-net-client test-net-server
	

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
