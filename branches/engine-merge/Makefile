CC=gcc
CFLAGS=-g -Wall -O3 `sdl-config --cflags`
LDFLAGS=`sdl-config --libs` -lSDL_ttf

EXECUTABLE=engine client server

all: $(EXECUTABLE)
	

################################################################################
# Build PBWafer
################################################################################
PBW_DIR=pbwafer/
PBW_SOURCES=$(addprefix pbwafer/,buf.c net.c cmd.c input.c proto.c list.c\
              vid_sdl.c utils.c view.c render_sdl.c menu.c console.c)
PBW_OBJECTS=$(PBW_SOURCES:.c=.o)

engine: $(PBW_OBJECTS)
	

################################################################################
# Build Client and Server
################################################################################
SOURCES=
OBJECTS=$(SOURCES:.c=.o)

client: $(OBJECTS) client.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(PBW_OBJECTS) $(OBJECTS) client.c -o $@
server: $(OBJECTS) server.c 
	$(CC) $(CFLAGS) $(LDFLAGS) $(PBW_OBJECTS) $(OBJECTS) server.c -o $@

################################################################################
# Build tests
################################################################################
T_EXECUTABLE=

test: $(T_EXECUTABLE)
	

################################################################################
# Implicit rule to compile all .c to .o
################################################################################
.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

################################################################################
# Cleanup rules
################################################################################
clean:
	rm -fv *.o object-gen/*.o $(PBW_DIR)*.o $(EXECUTABLE) $(T_EXECUTABLE) *~

clean-test:
	rm -fv *.o $(T_EXECUTABLE)  *~
