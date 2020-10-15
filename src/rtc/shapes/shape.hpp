//
// Created by USER on 14/09/2020.
//

#ifndef RAY_TRACER_SHAPE_HPP
#define RAY_TRACER_SHAPE_HPP
#include <rtc/core/collision_data.hpp>
#include <rtc/core/ray.hpp>
#include <glm/glm.hpp>
#include "rtc/core/material.h"
// Other cone functions:
//
// Cone bbox:         https://www.shadertoy.com/view/WdjSRK
// Cone distance:     https://www.shadertoy.com/view/tsSXzK
// Cone intersection: https://www.shadertoy.com/view/llcfRf
//
//
// Other intersectors: http://iquilezles.org/www/articles/intersectors/intersectors.htm
//
// Box:             https://www.shadertoy.com/view/ld23DV
// Triangle:        https://www.shadertoy.com/view/MlGcDz
// Capsule:         https://www.shadertoy.com/view/Xt3SzX
// Ellipsoid:       https://www.shadertoy.com/view/MlsSzn
// Sphere:          https://www.shadertoy.com/view/4d2XWV
// Capped Cylinder: https://www.shadertoy.com/view/4lcSRn
// Disk:            https://www.shadertoy.com/view/lsfGDB
// Capped Cone:     https://www.shadertoy.com/view/llcfRf
// Rounded Box:     https://www.shadertoy.com/view/WlSXRW
// Rounded Cone:    https://www.shadertoy.com/view/MlKfzm
// Torus:           https://www.shadertoy.com/view/4sBGDy
// Sphere4:         https://www.shadertoy.com/view/3tj3DW
// Goursat:         https://www.shadertoy.com/view/3lj3DW
// Hexagonal Prism: https://www.shadertoy.com/view/tljBWy


enum shape_type
{
    t_unknown,
    t_plane,
    t_disk,
    t_sphere,
    t_cube,
    t_cylinder,
    t_cone
};

struct shape{
    shape_type type=t_unknown;
    glm::mat4 model=glm::mat4();
    glm::mat4 model_inv=glm::mat4();
    material mat;
    virtual collision_data collide(ray r)=0;
    virtual glm::vec3 get_normal_at_point(glm::vec3 point,const collision_data& cache)const=0;
    void set_model(const glm::mat4& model)
    {
        this->model=model;
        this->model_inv=glm::inverse(model);
    }
    int id;
    shape(int id):id(id){}
};
#endif //RAY_TRACER_SHAPE_HPP
