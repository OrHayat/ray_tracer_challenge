//
// Created by USER on 14/09/2020.
//

#ifndef RAY_TRACER_SHAPE_HPP
#define RAY_TRACER_SHAPE_HPP
#include <rtc/core/collision_data.hpp>
#include <rtc/core/ray.hpp>
#include <glm/glm.hpp>
#include "rtc/core/material.h"

enum shape_type
{
    t_unknown,
    t_plane,
    t_disk,
    t_sphere,
    t_cube
};

struct shape{
    shape_type type=t_unknown;
    glm::mat4 model=glm::mat4();
    glm::mat4 model_inv=glm::mat4();
    material mat;
    virtual collision_data collide(ray r)=0;
    virtual glm::vec3 get_normal_at_point(glm::vec3 point)const=0;
    void set_model(const glm::mat4& model)
    {
        this->model=model;
        this->model_inv=glm::inverse(model);
    }
};
#endif //RAY_TRACER_SHAPE_HPP
