//
// Created by משתמש1 on 12/09/2020.
//

#ifndef RAY_TRACER_RAY_HPP
#define RAY_TRACER_RAY_HPP

#include <glm/glm.hpp>

typedef struct Ray//:public colider
{
    glm::vec3 dir;
    glm::vec3 pos;
    Ray(glm::vec3 dir,glm::vec3 pos):dir(glm::normalize(dir)),pos(pos)
    {}
}Ray;
#endif //RAY_TRACER_RAY_HPP
