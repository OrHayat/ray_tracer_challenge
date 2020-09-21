//
// Created by משתמש1 on 22/09/2020.
//

#ifndef RAY_TRACER_LIGHT_HPP
#define RAY_TRACER_LIGHT_HPP

#include <glm/glm.hpp>
struct light{
    glm::vec4 intensity=glm::vec4(0);
    glm::vec3 pos=glm::vec3(0);
    virtual glm::vec3 get_lighting_Intensity_from(glm::vec3 pos)=0;
};
#endif //RAY_TRACER_LIGHT_HPP
