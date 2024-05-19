#include "../include/linear.h"
#include "../include/font.h"
#include "../include/aabb.h"
#include "../include/octree.h"

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <time.h>

#define WINDOW_WIDTH  (800)
#define WINDOW_HEIGHT (800)

static SDL_Window *window;
static SDL_GLContext *context;
static SDL_Event event;
static int running;
static octree_t *octree;

#define OCTREE_LIMIT (1000)
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

int
main(int argc, char **argv)
{
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

	vec3_t from = ll_vec3_create3f(700.0, 300.0, -400.0),
		to = ll_vec3_create3f(250.0, 250.0, 250.0);
	vec3_t temp = ll_vec3_create3f(0.0, 1.0, 0.0);
	vec3_t forward = ll_vec3_normalise3fv(ll_vec3_sub3fv(from, to));
	vec3_t right = ll_vec3_cross3fv(temp, forward);
	vec3_t up = ll_vec3_cross3fv(forward, right);

	ll_matrix_mode(LL_MATRIX_PROJECTION);
	ll_matrix_perspective(90.0, WINDOW_WIDTH / WINDOW_HEIGHT,
			      10.0, 10000.0);
	glViewport(0.0, 0.0, WINDOW_WIDTH, WINDOW_HEIGHT);
	ll_matrix_mode(LL_MATRIX_VIEW);
	ll_matrix_lookat(right, up, forward, from);

	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	aabbs_init();
	
	running = 1;
	while (running) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = 0;
			} else if (event.type == SDL_KEYDOWN) {
				if (octree_limit < OCTREE_LIMIT) {
					octree_random_insert();
				}
			}
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		octree_render(octree);
		SDL_GL_SwapWindow(window);
	}
	SDL_Quit();
	return 0;
}
