#ifndef OCTREE_H_
#define OCTREE_H_

#include <stdio.h>
#include "linear.h"
#include "aabb.h"

#define OCTREE_LAYER_CAPACITY (100)
#define OCTREE_CHILDREN (8)

typedef struct octree_t {
	aabb_t aabb;
	size_t size;
	aabb_t objects[OCTREE_LAYER_CAPACITY];
	struct octree_t *children[OCTREE_CHILDREN];
} octree_t;

extern octree_t *
octree_create(aabb_t aabb);

extern int
octree_insert(octree_t *octree, aabb_t aabb);

extern aabb_t 
octree_find(octree_t *octree, ray_t ray);

extern void
octree_free(octree_t *octree);

extern void
octree_render(octree_t *octree);

#endif /* OCTREE_H_ */
