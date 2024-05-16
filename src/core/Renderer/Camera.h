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

void PerspectiveCamera_init(PerspectiveCamera *const perspective_camera, const float x, const float y);
void PerspectiveCamera_zoom(PerspectiveCamera *const PerspectiveCamera, const float zoom);
void PerspectiveCamera_set_fov(PerspectiveCamera *const perspective_camera, const float fov);
void PerspectiveCamera_set_aspect(PerspectiveCamera *const perspective_camera, const float aspect);
void PerspectiveCamera_set_near_z(PerspectiveCamera *const perspective_camera, const float near_z);
void PerspectiveCamera_set_far_z(PerspectiveCamera *const perspective_camera, const float far_z);
void PerspectiveCamera_set_id(PerspectiveCamera *const perspective_camera, const int speed);
void PerspectiveCamera_set_speed(PerspectiveCamera *const perspective_camera, const float speed);
void PerspectiveCamera_set_position(PerspectiveCamera *const perspective_camera, const vec3 position);
void PerspectiveCamera_set_looking_at(PerspectiveCamera *const perspective_camera, const vec3 looking_at);
void PerspectiveCamera_set_angles(PerspectiveCamera *const perspective_camera, const float yaw, const float pitch);
void PerspectiveCamera_change_direction(PerspectiveCamera *const perspective_camera, const float x, const float y);
void PerspectiveCamera_move_straight_up(PerspectiveCamera *const perspective_camera);
void PerspectiveCamera_move_straight_down(PerspectiveCamera *const perspective_camera);
void PerspectiveCamera_move_straight_front(PerspectiveCamera *const perspective_camera);
void PerspectiveCamera_move_straight_back(PerspectiveCamera *const perspective_camera);
void PerspectiveCamera_move_straight_right(PerspectiveCamera *const perspective_camera);
void PerspectiveCamera_move_straight_left(PerspectiveCamera *const perspective_camera);
void PerspectiveCamera_move_front(PerspectiveCamera *const perspective_camera);
void PerspectiveCamera_move_back(PerspectiveCamera *const perspective_camera);
void PerspectiveCamera_move_right(PerspectiveCamera *const perspective_camera);
void PerspectiveCamera_move_left(PerspectiveCamera *const perspective_camera);
float *PerspectiveCamera_get_view(const PerspectiveCamera *const perspective_camera);
float *PerspectiveCamera_get_projection(const PerspectiveCamera *const perspective_camera);

#endif