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

static void OB_update_perspective_projection(struct OB_Camera *camera) {
    assert(camera != NULL);

    OB_Lock_lock(&camera->lock);
    glm_perspective(camera->fov, camera->aspect, camera->near_z, camera->far_z, camera->projection);
    OB_Lock_unlock(&camera->lock);
}

void OB_Camera_init(struct OB_Camera *camera, float x, float y) {
    assert(camera != NULL);

    memset(camera, 0, sizeof(*camera));
    memcpy(camera->front, FRONT, sizeof(camera->front)); 
    camera->x = x;
    camera->y = y;
    camera->yaw = -90.0f;
    camera->pitch = 0.0f;
    camera->sensitivity = 0.05f;
    OB_Lock_init(&camera->lock);
    OB_update_view(camera);
    OB_update_perspective_projection(camera);
}

void OB_Camera_free(struct OB_Camera *camera) {
    assert(camera != NULL);

    OB_Lock_free(&camera->lock);
    memset(camera, 0, sizeof(*camera));
}

void OB_Camera_set_id(struct OB_Camera *camera, int id) {
    assert(camera != NULL);

    camera->id = id;
}

void OB_Camera_set_speed(struct OB_Camera *camera, float speed) {
    assert(camera != NULL);

    camera->speed = speed;
}

void OB_Camera_set_position(struct OB_Camera *camera, const vec3 position) {
    assert(camera != NULL);

    memcpy(camera->position, position, sizeof(vec3));
    OB_update_view(camera);
}

void OB_Camera_set_looking_at(struct OB_Camera *camera, const vec3 looking_at) {
    assert(camera != NULL);

    OB_Camera_set_front(camera, looking_at);
}

void OB_Camera_set_front(struct OB_Camera *camera, const vec3 front) {
    assert(camera != NULL);

    memcpy(camera->front, front, sizeof(vec3));
    OB_update_view(camera);
}

void OB_Camera_set_angles(struct OB_Camera *camera, float yaw, float pitch) {
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

void OB_Camera_change_direction(struct OB_Camera *camera, float x, float y) {
    assert(camera != NULL);

    const float yaw = camera->yaw + (x - camera->x) * camera->sensitivity;
    const float pitch = camera->pitch + (camera->y - y) * camera->sensitivity;
    camera->x = x;
    camera->y = y;
    OB_Camera_set_angles(camera, yaw, pitch);
}

void OB_Camera_move_up(struct OB_Camera *camera) {
    assert(camera != NULL);

    vec3 translate;
    glm_vec3_scale(UP, camera->speed, translate);
    glm_vec3_add(camera->position, translate, camera->position);
    OB_update_view(camera);
}

void OB_Camera_move_down(struct OB_Camera *camera) {
    assert(camera != NULL);

    vec3 translate;
    glm_vec3_scale(UP, camera->speed, translate);
    glm_vec3_sub(camera->position, translate, camera->position);
    OB_update_view(camera);
}

void OB_Camera_move_front(struct OB_Camera *camera) {
    assert(camera != NULL);

    vec3 translate;
    glm_vec3_scale(camera->front, camera->speed, translate);
    glm_vec3_add(camera->position, translate, camera->position);
    camera->position[Y] = 0.0f;
    OB_update_view(camera);
}

void OB_Camera_move_back(struct OB_Camera *camera) {
    assert(camera != NULL);

    vec3 translate;
    glm_vec3_scale(camera->front, camera->speed, translate);
    glm_vec3_sub(camera->position, translate, camera->position);
    camera->position[Y] = 0.0f;
    OB_update_view(camera);
}

void OB_Camera_move_right(struct OB_Camera *camera) {
    assert(camera != NULL);

    vec3 translate;
    glm_cross(camera->front, UP, translate);
    glm_normalize(translate);
    glm_vec3_scale(translate, camera->speed, translate);
    glm_vec3_add(camera->position, translate, camera->position);
    camera->position[Y] = 0.0f;
    OB_update_view(camera);
}

void OB_Camera_move_left(struct OB_Camera *camera) {
    assert(camera != NULL);

    vec3 translate;
    glm_cross(camera->front, UP, translate);
    glm_normalize(translate);
    glm_vec3_scale(translate, camera->speed, translate);
    glm_vec3_sub(camera->position, translate, camera->position);
    camera->position[Y] = 0.0f;
    OB_update_view(camera);
}

void OB_Camera_move_forward(struct OB_Camera *camera) {
    assert(camera != NULL);

    vec3 translate;
    glm_vec3_scale(camera->front, camera->speed, translate);
    glm_vec3_add(camera->position, translate, camera->position);
    OB_update_view(camera);
}

void OB_Camera_move_backward(struct OB_Camera *camera) {
    assert(camera != NULL);

    vec3 translate;
    glm_vec3_scale(camera->front, camera->speed, translate);
    glm_vec3_sub(camera->position, translate, camera->position);
    OB_update_view(camera);
}

void OB_Camera_strafe_right(struct OB_Camera *camera) {
    assert(camera != NULL);

    vec3 translate;
    glm_cross(camera->front, UP, translate);
    glm_normalize(translate);
    glm_vec3_scale(translate, camera->speed, translate);
    glm_vec3_add(camera->position, translate, camera->position);
    OB_update_view(camera);
}

void OB_Camera_strafe_left(struct OB_Camera *camera) {
    assert(camera != NULL);

    vec3 translate;
    glm_cross(camera->front, UP, translate);
    glm_normalize(translate);
    glm_vec3_scale(translate, camera->speed, translate);
    glm_vec3_sub(camera->position, translate, camera->position);
    OB_update_view(camera);
}

void OB_Camera_zoom(struct OB_Camera *camera, float zoom) {
    assert(camera != NULL);

    float fov = glm_deg(camera->fov);
    if(fov >= 1.0f && fov <= 45.0f) {
        fov -= (float)zoom;
    }
    if(fov <= 1.0f) {
        fov = 1.0f;
    }
    if(fov >= 45.0f) {
        fov = 45.0f;
    }
    OB_Camera_set_fov(camera, glm_rad(fov));
}

void OB_Camera_set_fov(struct OB_Camera *camera, float fov) {
    assert(camera != NULL);

    camera->fov = fov;
    OB_update_perspective_projection(camera);
}

void OB_Camera_set_aspect(struct OB_Camera *camera, float aspect) {
    assert(camera != NULL);

    camera->aspect = aspect;
    OB_update_perspective_projection(camera);
}

void OB_Camera_set_near_z(struct OB_Camera *camera, float near_z) {
    assert(camera != NULL);

    camera->near_z = near_z;
    OB_update_perspective_projection(camera);
}

void OB_Camera_set_far_z(struct OB_Camera *camera, float far_z) {
    assert(camera != NULL);

    camera->far_z = far_z;
    OB_update_perspective_projection(camera);
}

float *OB_Camera_get_view(struct OB_Camera *camera) {
    assert(camera != NULL);

    return (float*)camera->view;
}

float *OB_Camera_get_projection(struct OB_Camera *camera) {
    assert(camera != NULL);

    return (float*)camera->projection;
}