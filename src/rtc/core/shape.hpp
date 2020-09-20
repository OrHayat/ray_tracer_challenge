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
    glm::vec4 ka=glm::vec4(0);
    glm::vec4 ks=glm::vec4(0);;
    glm::vec4 kd=glm::vec4(0);;
    glm::vec4 kr=glm::vec4(0);;
    glm::vec4 kt=glm::vec4(0);;
    virtual collision_data collide(ray r)=0;
};
#endif //RAY_TRACER_SHAPE_HPP
