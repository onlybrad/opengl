//all opengl calls must be done in here
static void drawing_callback(struct OB_Window *window) {
    assert(window != NULL);

    const float time = OB_Window_time();
    const vec4 new_color = {
        MAX(0.2f, sinf(time * 2.0f)), 
        MAX(0.2f, sinf(time * 0.7f)), 
        MAX(0.2f, sinf(time * 1.3f)),
        1.0f
    };

    //The index of the light object in the current scene
    const unsigned light_index = 3;
    SceneObject3D *const scene_light = OB_Scene3D_object_get(window->scene, light_index);
    OB_Object_set_vec4_color(scene_light->object, (const float*)new_color);
    //Notify the scene that the light object has been updated, this will cause it to update the light object in the vertex buffer the next time OB_Scene3D_update_objects is called.
    OB_Scene3D_object_needs_update(window->scene, light_index);
    OB_Scene3D_update_objects(window->scene);

    OB_Shader_set_vec3(window->scene->shader, "light_position", scene_light->transform.translate);
    OB_Shader_set_vec4(window->scene->shader, "light_color", new_color);
    OB_Scene3D_draw_objects(window->scene);
}

//this runs on a seperate thread, opengl calls will fail if called from here. This is where CPU side logic must be implemented.
static void logic_callback(struct OB_Window *window) {
    assert(window != NULL);

    //The index of the light object in the current scene
    const unsigned light_index = 3;
    
    //Calculate the new rotation and position of the light source
    struct OB_Transform light_transform = OB_Scene3D_object_get(window->scene, light_index)->transform;
    light_transform.rotation_angle += glm_rad(1.0f);
    vec3 axis = {1.0f, 1.0f, 0.0f};
    glm_vec3_rotate(light_transform.translate, glm_rad(1.0f), axis);

    //update the Transform of the light object in the scene, this will automatically mark the light object as "needs_update" which will allow the drawing loop to update it in the vertex buffer once Scene3D_update_objects is called.
    OB_Scene3D_object_set_transform(window->scene, light_index, &light_transform);
}

