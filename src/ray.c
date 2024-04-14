#include "../include/ray.h"

ray_t
ray_create(vec3_t origin, vec3_t direction)
{
	ray_t ray;
	ray.origin = origin;
	ray.direction = ll_vec3_normalise3fv(direction);
	return ray;
}
