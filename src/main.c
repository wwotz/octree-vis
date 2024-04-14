#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

#define WINDOW_WIDTH  (800)
#define WINDOW_HEIGHT (800)

static SDL_Window *window;
static SDL_GLContext *context;
static SDL_Event event;
static int running;

int
main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Octree Visualisation",
				  SDL_WINDOWPOS_UNDEFINED,
				  SDL_WINDOWPOS_UNDEFINED,
				  WINDOW_WIDTH,
				  WINDOW_HEIGHT,
				  SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	context = SDL_GL_CreateContext(window);
	glewInit();
	
	running = 1;
	while (running) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = 0;
			}
		}
	}
	SDL_Quit();
	return 0;
}
