//
// Created by USER on 14/09/2020.
//

#ifndef RAY_TRACER_SHAPE_HPP
#define RAY_TRACER_SHAPE_HPP
#include <rtc/core/collision_data.hpp>
#include <rtc/core/ray.hpp>
struct shape{
    virtual collision_data collide(Ray r)=0;
};
#endif //RAY_TRACER_SHAPE_HPP
