#include "../include/aabb.h"

aabb_t
aabb_empty(void)
{
	return (aabb_t) {
		{{0.0, 0.0, 0.0}},
		{{0.0, 0.0, 0.0}}
	};
}

int
aabb_contains(aabb_t a, aabb_t b)
{
	for (int i = 0; i < 3; i++) {
		if (!((b.min.data[i] >= a.min.data[i] ||
		       fabsf(b.min.data[i] - a.min.data[i]) < 0.001) &&
		      (b.max.data[i] <= a.max.data[i] ||
		       fabsf(b.max.data[i] - a.max.data[i]) < 0.001))) return 0;
	}
	return 1;
}

// Slab Method for AABB - Ray Intersection. 
vec2_t
aabb_ray_intersect(ray_t ray, aabb_t aabb)
{
	int i;
	float t0, t1;
	float tmin, tmax;
	
	tmin = 0, tmax = INFINITY;
	for (i = 0; i < 3; i++) {
		t0 = (aabb.min.data[i] - ray.origin.data[i]) / ray.direction.data[i];
		t1 = (aabb.max.data[i] - ray.origin.data[i]) / ray.direction.data[i];

		tmin = fminf(fmaxf(t0, tmin), fmaxf(t1, tmin));
		tmax = fmaxf(fminf(t0, tmax), fminf(t1, tmax));
	}

	return ll_vec2_create2f(tmin, tmax);
}

int
aabb_ray_hit(ray_t ray, aabb_t aabb)
{
	vec2_t intersect = aabb_ray_intersect(ray, aabb);
	return intersect.x <= intersect.y;
}
