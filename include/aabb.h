#ifndef AABB_H_
#define AABB_H_

#include "linear.h"
#include "ray.h"

typedef struct aabb_t {
	vec3_t min;
	vec3_t max;
} aabb_t;

extern aabb_t
aabb_empty(void);

extern int
aabb_contains(aabb_t a, aabb_t b);

extern vec2_t
aabb_ray_intersect(ray_t ray, aabb_t aabb);

extern int
aabb_ray_hit(ray_t ray, aabb_t aabb);

#endif /* AABB_H_ */
