#include <string.h>
#include "Camera.h"

static vec3 FRONT = {0.0f, 0.0f, -1.0f};
static vec3 UP = {0.0f, 1.0f, 0.0f};

#define X 0
#define Y 1
#define Z 2

static void update_view(Camera camera[static 1]) {
    SYNCHRONIZE(&camera->lock,
    
    vec3 look_at;
    glm_vec3_add(camera->position, camera->front, look_at);
    glm_lookat(camera->position, look_at, UP, camera->view);
    
    )
}

static void update_perspective_projection(PerspectiveCamera perspective_camera[static 1]) {
    SYNCHRONIZE(&perspective_camera->camera.lock,

    glm_perspective(perspective_camera->fov, perspective_camera->aspect, perspective_camera->near_z, perspective_camera->far_z, perspective_camera->camera.projection);
    
    )
}

static void Camera_init(Camera camera[static 1], float x, float y) {
    memcpy(camera->front, FRONT, sizeof(vec3)); 
    camera->id = 0;
    camera->x = x;
    camera->y = y;
    camera->yaw = -90.0f;
    camera->pitch = 0.0f;
    camera->mouse_sensitivity = 0.05f;
    Lock_init(&camera->lock);
    update_view(camera);
}

static void Camera_free(Camera camera[static 1]) {
    Lock_free(&camera->lock);
}

inline static void Camera_set_id(Camera camera[static 1], const int id) {
    camera->id = id;
}

inline static void Camera_set_speed(Camera camera[static 1], const float speed) {
    camera->speed = speed;
}

inline static void Camera_set_position(Camera camera[static 1], const vec3 position) {
    memcpy(camera->position, position, sizeof(vec3));
    update_view(camera);
}

inline static void Camera_set_front(Camera camera[static 1], const vec3 front) {
    memcpy(camera->front, front, sizeof(vec3));
    update_view(camera);
}

static void Camera_set_angles(Camera camera[static 1], const float yaw, const float pitch) {
    camera->yaw = yaw;
    if(pitch > 89.0f) {
        camera->pitch = 89.0f;
    } else if(pitch < -89.0f) {
        camera->pitch = -89.0f;
    } else {
        camera->pitch = pitch;
    }
    camera->front[X] = cosf(glm_rad(yaw)) * cosf(glm_rad(camera->pitch));
    camera->front[Y] = sinf(glm_rad(camera->pitch));
    camera->front[Z] = sinf(glm_rad(yaw)) * cosf(glm_rad(camera->pitch));
    glm_normalize(camera->front);
    update_view(camera);
}

static void Camera_change_direction(Camera camera[static 1], const float x, const float y) {
    const float yaw = camera->yaw + (x - camera->x) * camera->mouse_sensitivity;
    const float pitch = camera->pitch + (camera->y - y) * camera->mouse_sensitivity;
    camera->x = x;
    camera->y = y;
    Camera_set_angles(camera, yaw, pitch);
}

static void Camera_move_straight_up(Camera camera[static 1]) {
    vec3 translate;
    glm_vec3_scale(UP, camera->speed, translate);
    glm_vec3_add(camera->position, translate, camera->position);
    update_view(camera);
}

static void Camera_move_straight_down(Camera camera[static 1]) {
    vec3 translate;
    glm_vec3_scale(UP, camera->speed, translate);
    glm_vec3_sub(camera->position, translate, camera->position);
    update_view(camera);
}

static void Camera_move_straight_front(Camera camera[static 1]) {
    vec3 translate;
    glm_vec3_scale(camera->front, camera->speed, translate);
    glm_vec3_add(camera->position, translate, camera->position);
    camera->position[Y] = 0.0f;
    update_view(camera);
}

static void Camera_move_straight_back(Camera camera[static 1]) {
    vec3 translate;
    glm_vec3_scale(camera->front, camera->speed, translate);
    glm_vec3_sub(camera->position, translate, camera->position);
    camera->position[Y] = 0.0f;
    update_view(camera);
}

static void Camera_move_straight_right(Camera camera[static 1]) {
    vec3 translate;
    glm_cross(camera->front, UP, translate);
    glm_normalize(translate);
    glm_vec3_scale(translate, camera->speed, translate);
    glm_vec3_add(camera->position, translate, camera->position);
    camera->position[Y] = 0.0f;
    update_view(camera);
}

static void Camera_move_straight_left(Camera camera[static 1]) {
    vec3 translate;
    glm_cross(camera->front, UP, translate);
    glm_normalize(translate);
    glm_vec3_scale(translate, camera->speed, translate);
    glm_vec3_sub(camera->position, translate, camera->position);
    camera->position[Y] = 0.0f;
    update_view(camera);
}

static void Camera_move_front(Camera camera[static 1]) {
    vec3 translate;
    glm_vec3_scale(camera->front, camera->speed, translate);
    glm_vec3_add(camera->position, translate, camera->position);
    update_view(camera);
}

static void Camera_move_back(Camera camera[static 1]) {
    vec3 translate;
    glm_vec3_scale(camera->front, camera->speed, translate);
    glm_vec3_sub(camera->position, translate, camera->position);
    update_view(camera);
}

static void Camera_move_right(Camera camera[static 1]) {
    vec3 translate;
    glm_cross(camera->front, UP, translate);
    glm_normalize(translate);
    glm_vec3_scale(translate, camera->speed, translate);
    glm_vec3_add(camera->position, translate, camera->position);
    update_view(camera);
}

static void Camera_move_left(Camera camera[static 1]) {
    vec3 translate;
    glm_cross(camera->front, UP, translate);
    glm_normalize(translate);
    glm_vec3_scale(translate, camera->speed, translate);
    glm_vec3_sub(camera->position, translate, camera->position);
    update_view(camera);
}

inline static float *Camera_get_view(const Camera camera[static 1]) {
    return (float*)camera->view;
}

inline static float *Camera_get_projection(const Camera camera[static 1]) {
    return (float*)camera->projection;
}

inline void PerspectiveCamera_init(PerspectiveCamera perspective_camera[static 1], const float x, const float y) {
    *perspective_camera = (PerspectiveCamera){0};
    Camera_init(&perspective_camera->camera, x, y);
    update_perspective_projection(perspective_camera);
}

inline void PerspectiveCamera_free(PerspectiveCamera perspective_camera[static 1]) {
    Camera_free(&perspective_camera->camera);
}

void PerspectiveCamera_zoom(PerspectiveCamera perspective_camera[static 1], const float zoom) {
    float fov = glm_deg(perspective_camera->fov);
    if(fov >= 1.0f && fov <= 45.0f) {
        fov -= (float)zoom;
    }
    if(fov <= 1.0f) {
        fov = 1.0f;
    }
    if(fov >= 45.0f) {
        fov = 45.0f;
    }
    PerspectiveCamera_set_fov(perspective_camera, glm_rad(fov));
}

void PerspectiveCamera_set_fov(PerspectiveCamera perspective_camera[static 1], const float fov) {
    perspective_camera->fov = fov;
    update_perspective_projection(perspective_camera);
}

void PerspectiveCamera_set_aspect(PerspectiveCamera perspective_camera[static 1], const float aspect) {
    perspective_camera->aspect = aspect;
    update_perspective_projection(perspective_camera);
}

void PerspectiveCamera_set_near_z(PerspectiveCamera perspective_camera[static 1], const float near_z) {
    perspective_camera->near_z = near_z;
    update_perspective_projection(perspective_camera);
}

void PerspectiveCamera_set_far_z(PerspectiveCamera perspective_camera[static 1], const float far_z) {
    perspective_camera->far_z = far_z;
    update_perspective_projection(perspective_camera);
}

void PerspectiveCamera_set_id(PerspectiveCamera perspective_camera[static 1], const int id) {
    Camera_set_id(&perspective_camera->camera, id);
}

inline void PerspectiveCamera_set_speed(PerspectiveCamera perspective_camera[static 1], const float speed) {
    Camera_set_speed(&perspective_camera->camera, speed);
}

inline void PerspectiveCamera_set_position(PerspectiveCamera perspective_camera[static 1], const vec3 position) {
    Camera_set_position(&perspective_camera->camera, position);
}

inline void PerspectiveCamera_set_looking_at(PerspectiveCamera perspective_camera[static 1], const vec3 looking_at) {
    Camera_set_front(&perspective_camera->camera, looking_at);
}

inline void PerspectiveCamera_set_angles(PerspectiveCamera perspective_camera[static 1], const float yaw, const float pitch) {
    Camera_set_angles(&perspective_camera->camera, yaw, pitch);
}

inline void PerspectiveCamera_change_direction(PerspectiveCamera perspective_camera[static 1], const float x, const float y) {
    Camera_change_direction(&perspective_camera->camera, x, y);
}

inline void PerspectiveCamera_move_straight_up(PerspectiveCamera perspective_camera[static 1]) {
    Camera_move_straight_up(&perspective_camera->camera);
}

inline void PerspectiveCamera_move_straight_down(PerspectiveCamera perspective_camera[static 1]) {
    Camera_move_straight_down(&perspective_camera->camera);
}

inline void PerspectiveCamera_move_straight_front(PerspectiveCamera perspective_camera[static 1]) {
    Camera_move_straight_front(&perspective_camera->camera);
}

inline void PerspectiveCamera_move_straight_back(PerspectiveCamera perspective_camera[static 1]) {
    Camera_move_straight_back(&perspective_camera->camera);
}

inline void PerspectiveCamera_move_straight_right(PerspectiveCamera perspective_camera[static 1]) {
    Camera_move_straight_right(&perspective_camera->camera);
}

inline void PerspectiveCamera_move_straight_left(PerspectiveCamera perspective_camera[static 1]) {
    Camera_move_straight_left(&perspective_camera->camera);
}

inline void PerspectiveCamera_move_front(PerspectiveCamera perspective_camera[static 1]) {
    Camera_move_front(&perspective_camera->camera);
}

inline void PerspectiveCamera_move_back(PerspectiveCamera perspective_camera[static 1]) {
    Camera_move_back(&perspective_camera->camera);
}

inline void PerspectiveCamera_move_right(PerspectiveCamera perspective_camera[static 1]) {
    Camera_move_right(&perspective_camera->camera);
}

inline void PerspectiveCamera_move_left(PerspectiveCamera perspective_camera[static 1]) {
    Camera_move_left(&perspective_camera->camera);
}

inline float *PerspectiveCamera_get_view(const PerspectiveCamera perspective_camera[static 1]) {
    return Camera_get_view(&perspective_camera->camera);
}

inline float *PerspectiveCamera_get_projection(const PerspectiveCamera perspective_camera[static 1]) {
    return Camera_get_projection(&perspective_camera->camera);
}