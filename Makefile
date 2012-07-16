SHELL = /bin/sh
CC  = /usr/bin/gcc-4.0
CXX = g++-4.0

# Edit as needed
FMOD_COMPILE_FLAGS = -I/usr/local/include/fmod/
FMOD_LINK_FLAGS = -lfmod -framework Carbon

CFLAGS = -O3 $(shell sdl-config --cflags) -arch i386
LDFLAGS = $(shell sdl-config --libs) -lSDL_image -arch i386

GAME = CandyCrisis
OBJECTGAME = blitter.o \
	control.o \
	font.o \
	fmodmusic.o \
	fmodsoundfx.o \
	gameticks.o \
	graphics.o \
	graymonitor.o \
	grays.o \
	gworld.o \
	hiscore.o \
	keyselect.o \
	level.o \
	main.o \
	moving.o \
	MTypes.o \
	next.o \
	opponent.o \
	pause.o \
	players.o \
	prefs.o \
	random.o \
	score.o \
	SDLU.o \
	soundfx.o \
	tutorial.o \
	tweak.o \
	victory.o \
	zap.o \
	RegAlgorithm.o

all: $(GAME)

.SUFFIXES: .o .cpp

%.o: %.cpp
	$(CXX) $(CFLAGS) $(FMOD_COMPILE_FLAGS) -c $< -o $@

$(GAME): $(OBJECTGAME)
	$(CXX) $(LDFLAGS) $(FMOD_LINK_FLAGS) -lstdc++ $^ -o $@

clean:
	$(RM) -fv $(OBJECTGAME)

distclean: clean
	$(RM) $(GAME)
