#ifndef OB_CAMERA_H
#define OB_CAMERA_H

#include <cglm/cglm.h>
#include "../thread/thread.h"

struct OB_Camera {
    int id;
    float x;
    float y;
    float yaw;
    float pitch;
    float speed;
    float sensitivity;
    float fov;
    float aspect;
    float near_z;
    float far_z;
    vec3 position;
    vec3 front;
    mat4 projection;
    mat4 view;
    struct OB_Lock lock;
};

void OB_Camera_init(struct OB_Camera *camera, float x, float y);
void OB_Camera_free(struct OB_Camera *camera);
void OB_Camera_zoom(struct OB_Camera *camera, float zoom);
void OB_Camera_set_fov(struct OB_Camera *camera, float fov);
void OB_Camera_set_aspect(struct OB_Camera *camera, float aspect);
void OB_Camera_set_near_z(struct OB_Camera *camera, float near_z);
void OB_Camera_set_far_z(struct OB_Camera *camera, float far_z);
void OB_Camera_set_id(struct OB_Camera *camera, int speed);
void OB_Camera_set_speed(struct OB_Camera *camera, float speed);
void OB_Camera_set_position(struct OB_Camera *camera, const vec3 position);
void OB_Camera_set_looking_at(struct OB_Camera *camera, const vec3 looking_at);
void OB_Camera_set_front(struct OB_Camera *camera, const vec3 front);
void OB_Camera_set_angles(struct OB_Camera *camera, float yaw, float pitch);
void OB_Camera_change_direction(struct OB_Camera *camera, float x, float y);
void OB_Camera_move_up(struct OB_Camera *camera);
void OB_Camera_move_down(struct OB_Camera *camera);
void OB_Camera_move_front(struct OB_Camera *camera);
void OB_Camera_move_back(struct OB_Camera *camera);
void OB_Camera_move_right(struct OB_Camera *camera);
void OB_Camera_move_left(struct OB_Camera *camera);
void OB_Camera_move_forward(struct OB_Camera *camera);
void OB_Camera_move_backward(struct OB_Camera *camera);
void OB_Camera_strafe_right(struct OB_Camera *camera);
void OB_Camera_strafe_left(struct OB_Camera *camera);
float *OB_Camera_get_view(struct OB_Camera *camera);
float *OB_Camera_get_projection(struct OB_Camera *camera);

#endif