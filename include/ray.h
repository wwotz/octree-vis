#ifndef RAY_H_
#define RAY_H_

#include "linear.h"

typedef struct ray_t {
	vec3_t origin;
	vec3_t direction;
} ray_t;

extern ray_t
ray_create(vec3_t origin, vec3_t direction);

#endif /* RAY_H_ */
