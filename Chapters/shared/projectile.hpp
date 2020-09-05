//
//

#ifndef RAY_TRACER_PROJECTILE_HPP
#define RAY_TRACER_PROJECTILE_HPP

#include <glm/glm.hpp>
#include "env.hpp"
namespace rtc {
    struct projectile
    {
        glm::vec3 position;
        glm::vec3 speed;
        projectile():position(glm::vec3(0)),speed(glm::vec3(0)){}
        projectile(glm::vec3 position,glm::vec3 speed):position(position),speed(speed){}
        void tick_(rtc::env e)
        {
            this->position+=speed;
            this->speed+=e.gravity+e.wind;

        }
        projectile tick(rtc::env e)
        {
            return  projectile(this->position+speed,this->speed+e.gravity+e.wind);
        }
    };
}
#endif //RAY_TRACER_PROJECTILE_HPP
