//
// Created by משתמש1 on 18/09/2020.
//

#ifndef RAY_TRACER_CAMERA_HPP
#define RAY_TRACER_CAMERA_HPP
#include <glm/glm.hpp>
#include <rtc/core/ray.hpp>
//https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-generating-camera-rays/generating-camera-rays
struct Camera {
    unsigned int image_width;
    unsigned int image_height;
    float fov;
    glm::mat4 camera_to_world_view=glm::mat4();
    float half_width;
    float half_height;
    float pixel_size;
    Camera(unsigned int hSize, unsigned  int vSize, float fov);
    ray Camera::RayForPixel(float x, float y) const;

};


#endif //RAY_TRACER_CAMERA_HPP
