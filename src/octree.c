#include "../include/octree.h"
#include <stdlib.h>

octree_t *
octree_create(aabb_t aabb)
{
	size_t i;
	octree_t *octree;
	octree = malloc(sizeof(*octree));
	if (!octree) {
		return NULL;
	}

	octree->size = 0;

	for (i = 0; i < OCTREE_LAYER_CAPACITY; i++) {
		octree->objects[i] = aabb_empty();
	}

	for (i = 0; i < OCTREE_CHILDREN; i++) {
		octree->children[i] = NULL;
	}

	octree->aabb = aabb;
	return octree;
}

static int
octree_contains(octree_t *octree, aabb_t aabb)
{
	return aabb_contains(octree->aabb, aabb);
}

// it's possible that something is so small that it can overflow the stack, so we
// limit how deep the octree can become so octree_insert_internal only goes to deep
#define OCTREE_MAXIMUM_DEPTH (5)

static int
octree_insert_internal(octree_t *octree, aabb_t aabb, int level)
{
	int i, j;
	vec3_t center;
	aabb_t quadrant;
	for (i = 0; i < 3; i++) {
		center.data[i] = (octree->aabb.min.data[i] + octree->aabb.max.data[i]) / 2.0;
	}

	for (i = 0; i < 8 && level < OCTREE_MAXIMUM_DEPTH; i++) {
		vec3_t qmin, qmax;
		for (j = 0; j < 3; j++) {
			if (i & (1<<j)) {
				qmin.data[j] = octree->aabb.min.data[j];
				qmax.data[j] = center.data[j];
			} else {
				qmin.data[j] = center.data[j];
				qmax.data[j] = octree->aabb.max.data[j];
			}
		}

		quadrant.min = qmin;
		quadrant.max = qmax;
		
		if (aabb_contains(quadrant, aabb)) {
			if (!octree->children[i]) {
				octree->children[i] = octree_create(quadrant);
				if (!octree->children[i]) {
					return -1;
				}
				
				return octree_insert_internal(octree->children[i], aabb, level+1);
			}
		}
	}

	if (octree->size == OCTREE_LAYER_CAPACITY) {
		return -1;
	}

	octree->objects[octree->size++] = aabb;
	return 0;
}

int
octree_insert(octree_t *octree, aabb_t aabb)
{
	return octree_insert_internal(octree, aabb, 0);
}

aabb_t 
octree_find(octree_t *octree, ray_t ray)
{
	int i, j;
	vec3_t center;
	vec2_t closest_intersect = ll_vec2_create2f(INFINITY, INFINITY), intersect;
	aabb_t quadrant, closest = aabb_empty();
	for (i = 0; i < 3; i++) {
		center.data[i] = (octree->aabb.min.data[i] + octree->aabb.max.data[i]) / 2.0;
	}
	
	for (i = 0; i < 8; i++) {
		vec3_t qmin, qmax;
		if (!octree->children[i]) continue;
		for (j = 0; j < 3; j++) {
			if (i & (1<<j)) {
				qmin.data[j] = octree->aabb.min.data[j];
				qmax.data[j] = center.data[j];
			} else {
				qmin.data[j] = center.data[j];
				qmax.data[j] = octree->aabb.max.data[j];
			}
		}

		quadrant.min = qmin;
		quadrant.max = qmax;

		intersect = aabb_ray_intersect(ray, quadrant);
		if (intersect.x <= intersect.y) {
			aabb_t this_closest = octree_find(octree->children[i], ray);
			intersect = aabb_ray_intersect(ray, this_closest);
			if (intersect.x <= intersect.y && intersect.x < closest_intersect.x) {
				closest_intersect = intersect;
				closest = this_closest;
			}
		}
	}

	for (i = 0; i < octree->size; i++) {
		aabb_t child = octree->objects[i];
		intersect = aabb_ray_intersect(ray, child);
		if (intersect.x <= intersect.y && intersect.x < closest_intersect.x) {
			closest_intersect = intersect;
			closest = child;
		}
	}

	return closest;
}

void
octree_free(octree_t *octree)
{
	int i;
	if (octree == NULL) return;
	for (i = 0; i < OCTREE_CHILDREN; i++) {
		octree_free(octree->children[i]);
	}
	free(octree);
}

static void
octree_render_internal(octree_t *octree)
{
	int i;
	if (octree == NULL) return;

	ll_matrix_mode(LL_MATRIX_MODEL);
	ll_matrix_identity();
	ll_matrix_scale3f(octree->aabb.max.x-octree->aabb.min.x,
			  octree->aabb.max.y-octree->aabb.min.y,
			  octree->aabb.max.z-octree->aabb.min.z);
	ll_matrix_translate3fv(octree->aabb.min);
	glUniformMatrix4fv(glGetUniformLocation(aabb_shader, "model"),
			   1, GL_FALSE, ll_matrix_get_copy().data);
	glUniform4f(glGetUniformLocation(aabb_shader, "colour"),
		    1.0, 1.0, 1.0, 1.0);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
		
	for (i = 0; i < octree->size; i++) {
		aabb_t aabb = octree->objects[i];
		ll_matrix_mode(LL_MATRIX_MODEL);
		ll_matrix_identity();
		ll_matrix_scale3f(aabb.max.x-aabb.min.x,
				  aabb.max.y-aabb.min.y,
				  aabb.max.z-aabb.min.z);
		ll_matrix_translate3fv(aabb.min);
		glUniformMatrix4fv(glGetUniformLocation(aabb_shader, "model"),
			   1, GL_FALSE, ll_matrix_get_copy().data);
		glUniform4f(glGetUniformLocation(aabb_shader, "colour"),
		    1.0, 1.0, 1.0, 1.0);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
	}

	for (i = 0; i < OCTREE_CHILDREN; i++) {
		octree_render_internal(octree->children[i]);
	}
}

void
octree_render(octree_t *octree)
{
	glUseProgram(aabb_shader);
	glBindVertexArray(aabb_buffers[AABB_BUFFER_VAO]);
	ll_matrix_mode(LL_MATRIX_PROJECTION);
	glUniformMatrix4fv(glGetUniformLocation(aabb_shader, "projection"),
			   1, GL_FALSE, ll_matrix_get_copy().data);
	ll_matrix_mode(LL_MATRIX_VIEW);
	glUniformMatrix4fv(glGetUniformLocation(aabb_shader, "view"),
			   1, GL_FALSE, ll_matrix_get_copy().data);
	octree_render_internal(octree);
	glBindVertexArray(0);
	glUseProgram(0);
}

