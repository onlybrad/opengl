#ifndef _CAMERA_H
#define _CAMERA_H

#include <cglm/cglm.h>
#include "../Thread/Thread.h"

typedef struct Camera {
    int id;
    float x;
    float y;
    float yaw;
    float pitch;
    float speed;
    float mouse_sensitivity;
    vec3 position;
    vec3 front;
    mat4 projection;
    mat4 view;
    Lock lock;
} Camera;

typedef struct PerspectiveCamera {
    float fov;
    float aspect;
    float near_z;
    float far_z;
    Camera camera;
} PerspectiveCamera;

void PerspectiveCamera_init(PerspectiveCamera perspective_camera[static 1], const float x, const float y);
void PerspectiveCamera_free(PerspectiveCamera perspective_camera[static 1]);
void PerspectiveCamera_zoom(PerspectiveCamera perspective_camera[static 1], const float zoom);
void PerspectiveCamera_set_fov(PerspectiveCamera perspective_camera[static 1], const float fov);
void PerspectiveCamera_set_aspect(PerspectiveCamera perspective_camera[static 1], const float aspect);
void PerspectiveCamera_set_near_z(PerspectiveCamera perspective_camera[static 1], const float near_z);
void PerspectiveCamera_set_far_z(PerspectiveCamera perspective_camera[static 1], const float far_z);
void PerspectiveCamera_set_id(PerspectiveCamera perspective_camera[static 1], const int speed);
void PerspectiveCamera_set_speed(PerspectiveCamera perspective_camera[static 1], const float speed);
void PerspectiveCamera_set_position(PerspectiveCamera perspective_camera[static 1], const vec3 position);
void PerspectiveCamera_set_looking_at(PerspectiveCamera perspective_camera[static 1], const vec3 looking_at);
void PerspectiveCamera_set_angles(PerspectiveCamera perspective_camera[static 1], const float yaw, const float pitch);
void PerspectiveCamera_change_direction(PerspectiveCamera perspective_camera[static 1], const float x, const float y);
void PerspectiveCamera_move_straight_up(PerspectiveCamera perspective_camera[static 1]);
void PerspectiveCamera_move_straight_down(PerspectiveCamera perspective_camera[static 1]);
void PerspectiveCamera_move_straight_front(PerspectiveCamera perspective_camera[static 1]);
void PerspectiveCamera_move_straight_back(PerspectiveCamera perspective_camera[static 1]);
void PerspectiveCamera_move_straight_right(PerspectiveCamera perspective_camera[static 1]);
void PerspectiveCamera_move_straight_left(PerspectiveCamera perspective_camera[static 1]);
void PerspectiveCamera_move_front(PerspectiveCamera perspective_camera[static 1]);
void PerspectiveCamera_move_back(PerspectiveCamera perspective_camera[static 1]);
void PerspectiveCamera_move_right(PerspectiveCamera perspective_camera[static 1]);
void PerspectiveCamera_move_left(PerspectiveCamera perspective_camera[static 1]);
float *PerspectiveCamera_get_view(const PerspectiveCamera perspective_camera[static 1]);
float *PerspectiveCamera_get_projection(const PerspectiveCamera perspective_camera[static 1]);

#endif