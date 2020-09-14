//
// Created by USER on 14/09/2020.
//

#ifndef RAY_TRACER_SPHERE_HPP
#define RAY_TRACER_SPHERE_HPP
#include <rtc/core/shape.hpp>
struct sphere:shape{
    float radius;
    glm::vec3 center;
    sphere(float radius,glm::vec3 center):radius(radius),center(center){};
    virtual collision_data collide(Ray r)
    {
        //x1x,x=(-b(+,-)sqrt(b^2-4ac))/2a when a=
    }
};
#endif //RAY_TRACER_SPHERE_HPP
