//
// Created by משתמש1 on 22/09/2020.
//

#ifndef RAY_TRACER_POINT_LIGHT_HPP
#define RAY_TRACER_POINT_LIGHT_HPP
#include "light.hpp"
#include <glm/glm.hpp>
struct point_light: public light{
    glm::vec3 intensity=glm::vec3(0);
    float kc = 1.0f;//constant
    float kl = 0.0f;//linear
    float kq = 0.0f;//quadratic
    virtual glm::vec3 get_lighting_Intensity_from(glm::vec3 object_pos)
    {
        float dist=glm::distance(pos,object_pos);
        return this->intensity/(((kq*dist)+kl)*dist+kc);
    };
};
#endif //RAY_TRACER_POINT_LIGHT_HPP
