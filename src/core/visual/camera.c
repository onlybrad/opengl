#include <assert.h>
#include <string.h>
#include "camera.h"

static vec3 FRONT = {0.0f, 0.0f, -1.0f};
static vec3 UP = {0.0f, 1.0f, 0.0f};

#define X 0
#define Y 1
#define Z 2

static void OB_update_view(struct OB_Camera *camera) {
    assert(camera != NULL);

    OB_Lock_lock(&camera->lock);
    vec3 look_at;
    glm_vec3_add(camera->position, camera->front, look_at);
    glm_lookat(camera->position, look_at, UP, camera->view);
    OB_Lock_unlock(&camera->lock);
}

static void OB_update_perspective_projection(struct OB_PerspectiveCamera *perspective_camera) {
    assert(perspective_camera != NULL);

    OB_Lock_lock(&perspective_camera->camera.lock);
    glm_perspective(perspective_camera->fov, perspective_camera->aspect, perspective_camera->near_z, perspective_camera->far_z, perspective_camera->camera.projection);
    OB_Lock_unlock(&perspective_camera->camera.lock);
}

static void OB_Camera_init(struct OB_Camera *camera, float x, float y) {
    assert(camera != NULL);

    memcpy(camera->front, FRONT, sizeof(vec3)); 
    camera->id = 0;
    camera->x = x;
    camera->y = y;
    camera->yaw = -90.0f;
    camera->pitch = 0.0f;
    camera->mouse_sensitivity = 0.05f;
    OB_Lock_init(&camera->lock);
    OB_update_view(camera);
}

static void OB_Camera_free(struct OB_Camera *camera) {
    assert(camera != NULL);

    OB_Lock_free(&camera->lock);
    memset(camera, 0, sizeof(*camera));
}

static void OB_Camera_set_id(struct OB_Camera *camera, int id) {
    assert(camera != NULL);

    camera->id = id;
}

static void OB_Camera_set_speed(struct OB_Camera *camera, float speed) {
    assert(camera != NULL);

    camera->speed = speed;
}

static void OB_Camera_set_position(struct OB_Camera *camera, const vec3 position) {
    assert(camera != NULL);

    memcpy(camera->position, position, sizeof(vec3));
    OB_update_view(camera);
}

static void OB_Camera_set_front(struct OB_Camera *camera, const vec3 front) {
    assert(camera != NULL);

    memcpy(camera->front, front, sizeof(vec3));
    OB_update_view(camera);
}

static void OB_Camera_set_angles(struct OB_Camera *camera, float yaw, float pitch) {
    assert(camera != NULL);

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
    OB_update_view(camera);
}

static void OB_Camera_change_direction(struct OB_Camera *camera, float x, float y) {
    assert(camera != NULL);

    const float yaw = camera->yaw + (x - camera->x) * camera->mouse_sensitivity;
    const float pitch = camera->pitch + (camera->y - y) * camera->mouse_sensitivity;
    camera->x = x;
    camera->y = y;
    OB_Camera_set_angles(camera, yaw, pitch);
}

static void OB_Camera_move_straight_up(struct OB_Camera *camera) {
    assert(camera != NULL);

    vec3 translate;
    glm_vec3_scale(UP, camera->speed, translate);
    glm_vec3_add(camera->position, translate, camera->position);
    OB_update_view(camera);
}

static void OB_Camera_move_straight_down(struct OB_Camera *camera) {
    assert(camera != NULL);

    vec3 translate;
    glm_vec3_scale(UP, camera->speed, translate);
    glm_vec3_sub(camera->position, translate, camera->position);
    OB_update_view(camera);
}

static void OB_Camera_move_straight_front(struct OB_Camera *camera) {
    assert(camera != NULL);

    vec3 translate;
    glm_vec3_scale(camera->front, camera->speed, translate);
    glm_vec3_add(camera->position, translate, camera->position);
    camera->position[Y] = 0.0f;
    OB_update_view(camera);
}

static void OB_Camera_move_straight_back(struct OB_Camera *camera) {
    assert(camera != NULL);

    vec3 translate;
    glm_vec3_scale(camera->front, camera->speed, translate);
    glm_vec3_sub(camera->position, translate, camera->position);
    camera->position[Y] = 0.0f;
    OB_update_view(camera);
}

static void OB_Camera_move_straight_right(struct OB_Camera *camera) {
    assert(camera != NULL);

    vec3 translate;
    glm_cross(camera->front, UP, translate);
    glm_normalize(translate);
    glm_vec3_scale(translate, camera->speed, translate);
    glm_vec3_add(camera->position, translate, camera->position);
    camera->position[Y] = 0.0f;
    OB_update_view(camera);
}

static void OB_Camera_move_straight_left(struct OB_Camera *camera) {
    assert(camera != NULL);

    vec3 translate;
    glm_cross(camera->front, UP, translate);
    glm_normalize(translate);
    glm_vec3_scale(translate, camera->speed, translate);
    glm_vec3_sub(camera->position, translate, camera->position);
    camera->position[Y] = 0.0f;
    OB_update_view(camera);
}

static void OB_Camera_move_front(struct OB_Camera *camera) {
    assert(camera != NULL);

    vec3 translate;
    glm_vec3_scale(camera->front, camera->speed, translate);
    glm_vec3_add(camera->position, translate, camera->position);
    OB_update_view(camera);
}

static void OB_Camera_move_back(struct OB_Camera *camera) {
    assert(camera != NULL);

    vec3 translate;
    glm_vec3_scale(camera->front, camera->speed, translate);
    glm_vec3_sub(camera->position, translate, camera->position);
    OB_update_view(camera);
}

static void OB_Camera_move_right(struct OB_Camera *camera) {
    assert(camera != NULL);

    vec3 translate;
    glm_cross(camera->front, UP, translate);
    glm_normalize(translate);
    glm_vec3_scale(translate, camera->speed, translate);
    glm_vec3_add(camera->position, translate, camera->position);
    OB_update_view(camera);
}

static void OB_Camera_move_left(struct OB_Camera *camera) {
    assert(camera != NULL);

    vec3 translate;
    glm_cross(camera->front, UP, translate);
    glm_normalize(translate);
    glm_vec3_scale(translate, camera->speed, translate);
    glm_vec3_sub(camera->position, translate, camera->position);
    OB_update_view(camera);
}

static float *OB_Camera_get_view(struct OB_Camera *camera) {
    assert(camera != NULL);

    return (float*)camera->view;
}

static float *OB_Camera_get_projection(struct OB_Camera *camera) {
    assert(camera != NULL);

    return (float*)camera->projection;
}

void OB_PerspectiveCamera_init(struct OB_PerspectiveCamera *perspective_camera, float x, float y) {
    assert(perspective_camera != NULL);

    memset(perspective_camera, 0, sizeof(*perspective_camera));
    OB_Camera_init(&perspective_camera->camera, x, y);
    OB_update_perspective_projection(perspective_camera);
}

void OB_PerspectiveCamera_free(struct OB_PerspectiveCamera *perspective_camera) {
    assert(perspective_camera != NULL);

    OB_Camera_free(&perspective_camera->camera);
    memset(perspective_camera, 0, sizeof(*perspective_camera));
}

void OB_PerspectiveCamera_zoom(struct OB_PerspectiveCamera *perspective_camera, float zoom) {
    assert(perspective_camera != NULL);

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
    OB_PerspectiveCamera_set_fov(perspective_camera, glm_rad(fov));
}

void OB_PerspectiveCamera_set_fov(struct OB_PerspectiveCamera *perspective_camera, float fov) {
    assert(perspective_camera != NULL);

    perspective_camera->fov = fov;
    OB_update_perspective_projection(perspective_camera);
}

void OB_PerspectiveCamera_set_aspect(struct OB_PerspectiveCamera *perspective_camera, float aspect) {
    assert(perspective_camera != NULL);

    perspective_camera->aspect = aspect;
    OB_update_perspective_projection(perspective_camera);
}

void OB_PerspectiveCamera_set_near_z(struct OB_PerspectiveCamera *perspective_camera, float near_z) {
    assert(perspective_camera != NULL);

    perspective_camera->near_z = near_z;
    OB_update_perspective_projection(perspective_camera);
}

void OB_PerspectiveCamera_set_far_z(struct OB_PerspectiveCamera *perspective_camera, float far_z) {
    assert(perspective_camera != NULL);

    perspective_camera->far_z = far_z;
    OB_update_perspective_projection(perspective_camera);
}

void OB_PerspectiveCamera_set_id(struct OB_PerspectiveCamera *perspective_camera, int id) {
    assert(perspective_camera != NULL);
    
    OB_Camera_set_id(&perspective_camera->camera, id);
}

void OB_PerspectiveCamera_set_speed(struct OB_PerspectiveCamera *perspective_camera, float speed) {
    assert(perspective_camera != NULL);

    OB_Camera_set_speed(&perspective_camera->camera, speed);
}

void OB_PerspectiveCamera_set_position(struct OB_PerspectiveCamera *perspective_camera, const vec3 position) {
    assert(perspective_camera != NULL);

    OB_Camera_set_position(&perspective_camera->camera, position);
}

void OB_PerspectiveCamera_set_looking_at(struct OB_PerspectiveCamera *perspective_camera, const vec3 looking_at) {
    assert(perspective_camera != NULL);

    OB_Camera_set_front(&perspective_camera->camera, looking_at);
}

void OB_PerspectiveCamera_set_angles(struct OB_PerspectiveCamera *perspective_camera, float yaw, float pitch) {
    assert(perspective_camera != NULL);

    OB_Camera_set_angles(&perspective_camera->camera, yaw, pitch);
}

void OB_PerspectiveCamera_change_direction(struct OB_PerspectiveCamera *perspective_camera, float x, float y) {
    assert(perspective_camera != NULL);

    OB_Camera_change_direction(&perspective_camera->camera, x, y);
}

void OB_PerspectiveCamera_move_straight_up(struct OB_PerspectiveCamera *perspective_camera) {
    assert(perspective_camera != NULL);

    OB_Camera_move_straight_up(&perspective_camera->camera);
}

void OB_PerspectiveCamera_move_straight_down(struct OB_PerspectiveCamera *perspective_camera) {
    assert(perspective_camera != NULL);

    OB_Camera_move_straight_down(&perspective_camera->camera);
}

void OB_PerspectiveCamera_move_straight_front(struct OB_PerspectiveCamera *perspective_camera) {
    assert(perspective_camera != NULL);

    OB_Camera_move_straight_front(&perspective_camera->camera);
}

void OB_PerspectiveCamera_move_straight_back(struct OB_PerspectiveCamera *perspective_camera) {
    assert(perspective_camera != NULL);

    OB_Camera_move_straight_back(&perspective_camera->camera);
}

void OB_PerspectiveCamera_move_straight_right(struct OB_PerspectiveCamera *perspective_camera) {
    assert(perspective_camera != NULL);

    OB_Camera_move_straight_right(&perspective_camera->camera);
}

void OB_PerspectiveCamera_move_straight_left(struct OB_PerspectiveCamera *perspective_camera) {
    assert(perspective_camera != NULL);

    OB_Camera_move_straight_left(&perspective_camera->camera);
}

void OB_PerspectiveCamera_move_front(struct OB_PerspectiveCamera *perspective_camera) {
    assert(perspective_camera != NULL);

    OB_Camera_move_front(&perspective_camera->camera);
}

void OB_PerspectiveCamera_move_back(struct OB_PerspectiveCamera *perspective_camera) {
    assert(perspective_camera != NULL);

    OB_Camera_move_back(&perspective_camera->camera);
}

void OB_PerspectiveCamera_move_right(struct OB_PerspectiveCamera *perspective_camera) {
    assert(perspective_camera != NULL);

    OB_Camera_move_right(&perspective_camera->camera);
}

void OB_PerspectiveCamera_move_left(struct OB_PerspectiveCamera *perspective_camera) {
    assert(perspective_camera != NULL);

    OB_Camera_move_left(&perspective_camera->camera);
}

float *OB_PerspectiveCamera_get_view(struct OB_PerspectiveCamera *perspective_camera) {
    assert(perspective_camera != NULL);

    return OB_Camera_get_view(&perspective_camera->camera);
}

float *OB_PerspectiveCamera_get_projection(struct OB_PerspectiveCamera *perspective_camera) {
    assert(perspective_camera != NULL);

    return OB_Camera_get_projection(&perspective_camera->camera);
}