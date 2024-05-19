#ifndef CAMERA_H_
#define CAMERA_H_

#include "linear.h"

#define CAMERA_DELTA (5.0)

extern void 
camera_setup(vec3_t from, vec3_t to);

extern void
camera_move_left(void);

extern void
camera_move_right(void);

extern void
camera_move_up(void);

extern void
camera_move_down(void);

extern void
camera_move_forward(void);

extern void
camera_move_backward(void);

extern void
camera_rotate(float angle);

#endif /* CAMERA_H_ */
