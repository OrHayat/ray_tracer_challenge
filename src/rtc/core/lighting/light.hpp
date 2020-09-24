//
// Created by משתמש1 on 22/09/2020.
//

#ifndef RAY_TRACER_LIGHT_HPP
#define RAY_TRACER_LIGHT_HPP

#include <glm/glm.hpp>

enum light_type
{
    unknown_type,
    point_light,
    directed_light,
    spot_light
};
struct light{
    light_type type;
    glm::vec3 pos;
    glm::vec3 dir;
    glm::vec4 intensity;
    float kc;// = 1.0f;//constant
    float kl;// = 0.0f;//linear
    float kq;// = 0.0f;//quadratic
    float cutoff_angel;
    light(light_type type):type(type){}


   static light* make_point_light_ptr(glm::vec3 pos,float kc=1.0f,float kl=0.0f,float kq=0.0f,glm::vec4 intensity=glm::vec4(0))
    {
        light* res= new light(light_type::point_light);
        res->pos=pos;
        res->kc=kc;
        res->kl=kl;
        res->kq=kq;
        res->intensity=intensity;
        return res;
    }

    static light* make_directed_light_ptr(glm::vec3 dir,glm::vec4 intensity=glm::vec4(0))
    {
        light* res= new light(light_type::directed_light);
        res->dir=dir;
        res->intensity=intensity;
        return res;
    }
    static  light* make_spot_light_ptr()
    {
        return new light(light_type::spot_light);
    }
};

#endif //RAY_TRACER_LIGHT_HPP
