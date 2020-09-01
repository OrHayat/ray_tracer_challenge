

#ifndef RAY_TRACER_ENV_HPP
#define RAY_TRACER_ENV_HPP

#include <glm/glm.hpp>
namespace rtc{
    struct env
    {
        glm::vec3 gravity;
        glm::vec3 wind;
        env():gravity(glm::vec3(0)),wind(glm::vec3(0)){};
        env(glm::vec3 gravity,glm::vec3 wind):gravity(gravity),wind(wind){};
    };
}

#endif //RAY_TRACER_ENV_HPP
