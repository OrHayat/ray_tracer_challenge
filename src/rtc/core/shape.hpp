//
// Created by USER on 14/09/2020.
//

#ifndef RAY_TRACER_SHAPE_HPP
#define RAY_TRACER_SHAPE_HPP
#include <rtc/core/collision_data.hpp>
#include <rtc/core/ray.hpp>
#include <glm/glm.hpp>
struct shape{
    glm::mat4 model;
    virtual collision_data collide(ray r)=0;
};
#endif //RAY_TRACER_SHAPE_HPP
