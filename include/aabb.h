#ifndef AABB_H_
#define AABB_H_

#include "linear.h"
#include "ray.h"
#include <GL/glew.h>

typedef enum aabb_buffer_t {
	AABB_BUFFER_VAO,
	AABB_BUFFER_VBO,
	AABB_BUFFER_EBO,
	AABB_BUFFER_COUNT
} aabb_buffer_t;

extern aabb_buffer_t aabb_buffers[AABB_BUFFER_COUNT];
extern GLuint aabb_shader;

typedef struct aabb_t {
	vec3_t min;
	vec3_t max;
} aabb_t;

extern int
aabbs_init(void);

extern aabb_t
aabb_empty(void);

extern int
aabb_contains(aabb_t a, aabb_t b);

extern vec2_t
aabb_ray_intersect(ray_t ray, aabb_t aabb);

extern int
aabb_ray_hit(ray_t ray, aabb_t aabb);

#endif /* AABB_H_ */
