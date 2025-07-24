#ifndef OB_CAMERA_H
#define OB_CAMERA_H

#include <cglm/cglm.h>
#include "../Thread/Thread.h"

struct OB_Camera {
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
    struct OB_Lock lock;
};

struct OB_PerspectiveCamera {
    struct OB_Camera camera;
    float fov;
    float aspect;
    float near_z;
    float far_z;
};

void OB_PerspectiveCamera_init(struct OB_PerspectiveCamera *perspective_camera, float x, float y);
void OB_PerspectiveCamera_free(struct OB_PerspectiveCamera *perspective_camera);
void OB_PerspectiveCamera_zoom(struct OB_PerspectiveCamera *perspective_camera, float zoom);
void OB_PerspectiveCamera_set_fov(struct OB_PerspectiveCamera *perspective_camera, float fov);
void OB_PerspectiveCamera_set_aspect(struct OB_PerspectiveCamera *perspective_camera, float aspect);
void OB_PerspectiveCamera_set_near_z(struct OB_PerspectiveCamera *perspective_camera, float near_z);
void OB_PerspectiveCamera_set_far_z(struct OB_PerspectiveCamera *perspective_camera, float far_z);
void OB_PerspectiveCamera_set_id(struct OB_PerspectiveCamera *perspective_camera, int speed);
void OB_PerspectiveCamera_set_speed(struct OB_PerspectiveCamera *perspective_camera, float speed);
void OB_PerspectiveCamera_set_position(struct OB_PerspectiveCamera *perspective_camera, const vec3 position);
void OB_PerspectiveCamera_set_looking_at(struct OB_PerspectiveCamera *perspective_camera, const vec3 looking_at);
void OB_PerspectiveCamera_set_angles(struct OB_PerspectiveCamera *perspective_camera, float yaw, float pitch);
void OB_PerspectiveCamera_change_direction(struct OB_PerspectiveCamera *perspective_camera, float x, float y);
void OB_PerspectiveCamera_move_straight_up(struct OB_PerspectiveCamera *perspective_camera);
void OB_PerspectiveCamera_move_straight_down(struct OB_PerspectiveCamera *perspective_camera);
void OB_PerspectiveCamera_move_straight_front(struct OB_PerspectiveCamera *perspective_camera);
void OB_PerspectiveCamera_move_straight_back(struct OB_PerspectiveCamera *perspective_camera);
void OB_PerspectiveCamera_move_straight_right(struct OB_PerspectiveCamera *perspective_camera);
void OB_PerspectiveCamera_move_straight_left(struct OB_PerspectiveCamera *perspective_camera);
void OB_PerspectiveCamera_move_front(struct OB_PerspectiveCamera *perspective_camera);
void OB_PerspectiveCamera_move_back(struct OB_PerspectiveCamera *perspective_camera);
void OB_PerspectiveCamera_move_right(struct OB_PerspectiveCamera *perspective_camera);
void OB_PerspectiveCamera_move_left(struct OB_PerspectiveCamera *perspective_camera);
float *OB_PerspectiveCamera_get_view(struct OB_PerspectiveCamera *perspective_camera);
float *OB_PerspectiveCamera_get_projection(struct OB_PerspectiveCamera *perspective_camera);

#endif