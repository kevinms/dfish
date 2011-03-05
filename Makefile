CC=gcc

ifdef SND
CFLAGS=-g -Wall -O3 `sdl-config --cflags` -D SOUND
LDFLAGS=`sdl-config --libs` -lSDL_ttf -lSDL_mixer
else
CFLAGS=-g -Wall -O3 `sdl-config --cflags`
LDFLAGS=`sdl-config --libs` -lSDL_ttf
endif

EXECUTABLE=engine client server

all: $(EXECUTABLE)
	

################################################################################
# Build PBWafer
################################################################################
PBW_DIR=pbwafer/

ifdef SND
PBW_SOUND=snd_sdl.c
else
PBW_SOUND=
endif

PBW_SOURCES_OPTIONAL=$(PBW_SOUND)

PBW_SOURCES=$(addprefix pbwafer/,buf.c net.c cmd.c input.c proto.c list.c\
              vid_sdl.c utils.c view.c render_sdl.c menu.c console.c pbwafer.c\
              blueprint.c timer.c $(PBW_SOURCES_OPTIONAL))
PBW_OBJECTS=$(PBW_SOURCES:.c=.o)

engine: $(PBW_OBJECTS)
	

################################################################################
# Build Client and Server
################################################################################
SOURCES= view_space.c view_console.c view_fabricator.c
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
