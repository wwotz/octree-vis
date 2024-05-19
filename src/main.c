#include "../include/settings.h"
#include "../include/linear.h"
#include "../include/font.h"
#include "../include/aabb.h"
#include "../include/octree.h"
#include "../include/camera.h"

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <time.h>

static SDL_Window *window;
static SDL_GLContext *context;
static SDL_Event event;
static int running;
static octree_t *octree;

size_t octree_limit;

static void
octree_random_insert(void)
{
	float a, b, c, d, e, f;
	a = ((random() / (float) RAND_MAX) * 450.0);
	b = ((random() / (float) RAND_MAX) * 450.0);
	c = ((random() / (float) RAND_MAX) * 450.0);
	d = a + ((random() / (float) RAND_MAX) * 30.0) + 10.0;
	e = b + ((random() / (float) RAND_MAX) * 30.0) + 10.0;
	f = c + ((random() / (float) RAND_MAX) * 30.0) + 10.0;
	octree_insert(octree, (aabb_t) { {{a,b,c}}, {{d,e,f}}});
}

enum button_pressed_t {
	BUTTON_PRESSED_W,
	BUTTON_PRESSED_A,
	BUTTON_PRESSED_S,
	BUTTON_PRESSED_D,
	BUTTON_PRESSED_Z,
	BUTTON_PRESSED_X,
	BUTTON_PRESSED_COUNT
};

int
main(int argc, char **argv)
{
	int i;
	int buttons[BUTTON_PRESSED_COUNT];
	for (i = 0; i < BUTTON_PRESSED_COUNT; i++)
		buttons[i] = 0;
	vec3_t from = ll_vec3_create3f(700.0, 300.0, -400.0),
		to = ll_vec3_create3f(250.0, 250.0, 250.0);
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);
	window = SDL_CreateWindow("Octree Visualisation",
				  SDL_WINDOWPOS_UNDEFINED,
				  SDL_WINDOWPOS_UNDEFINED,
				  WINDOW_WIDTH,
				  WINDOW_HEIGHT,
				  SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	context = SDL_GL_CreateContext(window);
	glewInit();

	octree = octree_create((aabb_t) {
			{{ 0.0, 0.0, 0.0 }},
			{{ 500.0, 500.0, 500.0 }}
		});

	camera_setup(from,to);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	aabbs_init();
	
	running = 1;
	while (running) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = 0;
			} else if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
				case SDLK_w:
					buttons[BUTTON_PRESSED_W] = 1;
					break;
				case SDLK_a:
					buttons[BUTTON_PRESSED_A] = 1;
					break;
				case SDLK_s:
					buttons[BUTTON_PRESSED_S] = 1;
					break;
				case SDLK_d:
					buttons[BUTTON_PRESSED_D] = 1;
					break;
				case SDLK_z:
					buttons[BUTTON_PRESSED_Z] = 1;
					break;
				case SDLK_x:
					buttons[BUTTON_PRESSED_X] = 1;
					break;
				case SDLK_TAB:
					if (octree_limit < OCTREE_LIMIT) {
						octree_random_insert();
					}
					break;
				}
			} else if (event.type == SDL_KEYUP) {
				switch (event.key.keysym.sym) {
				case SDLK_w:
					buttons[BUTTON_PRESSED_W] = 0;
					break;
				case SDLK_a:
					buttons[BUTTON_PRESSED_A] = 0;
					break;
				case SDLK_s:
					buttons[BUTTON_PRESSED_S] = 0;
					break;
				case SDLK_d:
					buttons[BUTTON_PRESSED_D] = 0;
					break;
				case SDLK_z:
					buttons[BUTTON_PRESSED_Z] = 0;
					break;
				case SDLK_x:
					buttons[BUTTON_PRESSED_X] = 0;
					break;
				}
			}
		}

		if (buttons[BUTTON_PRESSED_W]) camera_move_up();
		if (buttons[BUTTON_PRESSED_A]) camera_move_left();
		if (buttons[BUTTON_PRESSED_S]) camera_move_down();
		if (buttons[BUTTON_PRESSED_D]) camera_move_right();
		if (buttons[BUTTON_PRESSED_Z]) camera_move_forward();
		if (buttons[BUTTON_PRESSED_X]) camera_move_backward();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		octree_render(octree);
		SDL_GL_SwapWindow(window);
	}
	SDL_Quit();
	return 0;
}
