CC = gcc
CFLAGS = `pkg-config --cflags sdl2 SDL2_image freetype2 glew` -Wall
CLIBS = `pkg-config --libs sdl2 SDL2_image freetype2 glew` -lm
SRC = $(wildcard src/*.c)
OBJ = $(patsubst %.c, %.o, $(SRC))
EXEC = bin/octree-vis

all: $(EXEC)

%.o : %.c
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo "CC $<"

$(EXEC) : $(OBJ)
	@mkdir bin
	@$(CC) -o $@ $^ $(CLIBS) $(CFLAGS)
	@echo "Finished compiling the visualisation."
clean:
	@rm -rf bin
	@rm src/*.o
	@echo "Cleaned up the build."
