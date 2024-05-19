#include "../include/camera.h"
#include "../include/settings.h"

#include <GL/glew.h>

static vec3_t pos;
static vec3_t lookat;

extern void 
camera_setup(vec3_t from, vec3_t to)
{
	pos = from;
	lookat = to;
	vec3_t temp = ll_vec3_create3f(0.0, 1.0, 0.0);
	vec3_t forward = ll_vec3_normalise3fv(ll_vec3_sub3fv(pos, lookat));
	vec3_t right = ll_vec3_cross3fv(temp, forward);
	vec3_t up = ll_vec3_cross3fv(forward, right);

	ll_matrix_mode(LL_MATRIX_PROJECTION);
	ll_matrix_mode(LL_MATRIX_PROJECTION);
	ll_matrix_perspective(90.0, WINDOW_WIDTH / WINDOW_HEIGHT,
			      10.0, 10000.0);
	glViewport(0.0, 0.0, WINDOW_WIDTH, WINDOW_HEIGHT);
	ll_matrix_mode(LL_MATRIX_VIEW);
	ll_matrix_lookat(right, up, forward, pos);
	
}

extern void
camera_move_left(void)
{
	pos.x += CAMERA_DELTA;
	vec3_t temp = ll_vec3_create3f(0.0, 1.0, 0.0);
	vec3_t forward = ll_vec3_normalise3fv(ll_vec3_sub3fv(pos, lookat));
	vec3_t right = ll_vec3_cross3fv(temp, forward);
	vec3_t up = ll_vec3_cross3fv(forward, right);
	ll_matrix_mode(LL_MATRIX_VIEW);
	ll_matrix_lookat(right, up, forward, pos);
}

extern void
camera_move_right(void)
{
	pos.x -= CAMERA_DELTA;
	vec3_t temp = ll_vec3_create3f(0.0, 1.0, 0.0);
	vec3_t forward = ll_vec3_normalise3fv(ll_vec3_sub3fv(pos, lookat));
	vec3_t right = ll_vec3_cross3fv(temp, forward);
	vec3_t up = ll_vec3_cross3fv(forward, right);
	ll_matrix_mode(LL_MATRIX_VIEW);
	ll_matrix_lookat(right, up, forward, pos);
}

extern void
camera_move_up(void)
{
	pos.y -= CAMERA_DELTA;
	vec3_t temp = ll_vec3_create3f(0.0, 1.0, 0.0);
	vec3_t forward = ll_vec3_normalise3fv(ll_vec3_sub3fv(pos, lookat));
	vec3_t right = ll_vec3_cross3fv(temp, forward);
	vec3_t up = ll_vec3_cross3fv(forward, right);
	ll_matrix_mode(LL_MATRIX_VIEW);
	ll_matrix_lookat(right, up, forward, pos);
}

extern void
camera_move_down(void)
{
	pos.y += CAMERA_DELTA;
	vec3_t temp = ll_vec3_create3f(0.0, 1.0, 0.0);
	vec3_t forward = ll_vec3_normalise3fv(ll_vec3_sub3fv(pos, lookat));
	vec3_t right = ll_vec3_cross3fv(temp, forward);
	vec3_t up = ll_vec3_cross3fv(forward, right);
	ll_matrix_mode(LL_MATRIX_VIEW);
	ll_matrix_lookat(right, up, forward, pos);
}

extern void
camera_move_forward(void)
{
	pos.z += CAMERA_DELTA;
	vec3_t temp = ll_vec3_create3f(0.0, 1.0, 0.0);
	vec3_t forward = ll_vec3_normalise3fv(ll_vec3_sub3fv(pos, lookat));
	vec3_t right = ll_vec3_cross3fv(temp, forward);
	vec3_t up = ll_vec3_cross3fv(forward, right);
	ll_matrix_mode(LL_MATRIX_VIEW);
	ll_matrix_lookat(right, up, forward, pos);
}

extern void
camera_move_backward(void)
{
	pos.z -= CAMERA_DELTA;
	vec3_t temp = ll_vec3_create3f(0.0, 1.0, 0.0);
	vec3_t forward = ll_vec3_normalise3fv(ll_vec3_sub3fv(pos, lookat));
	vec3_t right = ll_vec3_cross3fv(temp, forward);
	vec3_t up = ll_vec3_cross3fv(forward, right);
	ll_matrix_mode(LL_MATRIX_VIEW);
	ll_matrix_lookat(right, up, forward, pos);
}

extern void
camera_rotate(float angle)
{

}
