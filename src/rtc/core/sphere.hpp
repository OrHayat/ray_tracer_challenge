//
// Created by USER on 14/09/2020.
//

#ifndef RAY_TRACER_SPHERE_HPP
#define RAY_TRACER_SPHERE_HPP
#include <rtc/core/shape.hpp>
#include <rtc/core/ray.hpp>
#include <iostream>

struct sphere:shape{
    glm::vec3 center;
    float radius;
    sphere():center(glm::vec3(0)),radius(1.0f){}
    sphere(float radius):center(glm::vec3(0)),radius(radius){}//initalzing sphere on center of world
    sphere(float radius,glm::vec3 center):center(center),radius(radius){};
    glm::vec3 get_normal_at_point(glm::vec3 point)
    {
        glm::vec3 tmp=glm::transpose(this->model_inv)*glm::vec4(point-(this->model*glm::vec4(center,1)).xyz(),0);
        return glm::normalize(tmp);
    }
    virtual collision_data collide(ray r)
    {
//        glm::mat4 model_inv=glm::inverse(this->model);
        glm::vec4 new_dir= this->model_inv*glm::vec4(r.dir,0);
        glm::vec4 new_pos= this->model_inv*glm::vec4(r.pos,1);
        r.pos=new_pos;
        r.dir=new_dir;
//        r=ray(new_pos,new_dir);
        //x1x,x=(-b(+,-)sqrt(b^2-4ac))/2a when dot(a,a)=1 and b=2*dot(r.dir,sphere to origin) then=x1,x2=-b(+,-)sqrt(b^2-c)
        float a=glm::dot(r.dir,r.dir);
        float b=glm::dot((r.dir),(r.origin-center));
        float c=glm::dot((r.origin-center),(r.origin-center))-radius*radius;
        collision_data res(*this);
        if (c > 0.0f && b > 0.0f) return res;
        float disc=b*b-a*c;
        if(disc<0)
        {
            return res;
        }
        disc=std::sqrt(disc);
        res.t.push_back((-b-disc)/a);
        res.t.push_back((-b+disc)/a);
//        std::cout<<"found collision t="<<res.t[0]<<","<<res.t[1]<<std::endl;
        return res;
    }
};
#endif //RAY_TRACER_SPHERE_HPP
