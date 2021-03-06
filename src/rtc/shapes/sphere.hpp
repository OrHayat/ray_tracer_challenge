//
// Created by USER on 14/09/2020.
//

#ifndef RAY_TRACER_SPHERE_HPP
#define RAY_TRACER_SPHERE_HPP
#include <rtc/shapes/shape.hpp>
#include <rtc/core/ray.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

struct sphere:shape{
    glm::vec3 center;
    float radius;
    sphere(int id):shape(id),center(glm::vec3(0)),radius(1.0f){
        this->type=shape_type::t_sphere;
    }
    explicit sphere(int id,float radius):shape(id),center(glm::vec3(0)),radius(radius)
    {
        this->type=shape_type::t_sphere;
    }//initalzing sphere on center of world
    sphere(int id,float radius,const glm::vec3& center):shape(id),center(center),radius(radius)
    {
        this->type=shape_type::t_sphere;
    };
    glm::vec3 get_normal_at_point(glm::vec3 point,const collision_data& cache)const override
    {
        glm::vec4 objectPoint = this->model_inv*glm::vec4(point,1);

        glm::vec4 object_normal=glm::vec4(objectPoint.xyz()-this->center,0);
        glm::vec3 worldnormal=(glm::transpose(this->model_inv)*object_normal).xyz;
        return glm::normalize(worldnormal);
    }
    collision_data collide(ray r) override// collision_data collide(ray r)
    {
//        glm::mat4 model_inv=glm::inverse(this->model);
        glm::vec4 new_dir= this->model_inv*glm::vec4(r.dir,0);
        glm::vec4 new_pos= this->model_inv*glm::vec4(r.pos,1);
        r.pos=new_pos;
        r.dir=new_dir;
//        r=ray(new_pos,new_dir);
        //x1x,x=(-b(+,-)sqrt(b^2-4ac))/2a when dot(a,a)=1 and b=2*dot(r.dir,sphere to origin) then=x1,x2=-b(+,-)sqrt(b^2-c)
//        float a=glm::dot(r.dir,r.dir);
        float b=glm::dot((r.dir),(r.origin-center));
        float c=glm::dot((r.origin-center),(r.origin-center))-radius*radius;
        collision_data res(*this);
        float disc=b*b-c;
        if(disc<0)
        {
            return res;
        }
        disc=std::sqrt(disc);
        float tmp=-b-disc;
        if(tmp>=0)
        {
            res.t.push_back(tmp);
            tmp=-b+disc;
            res.t.push_back(tmp);
            return res;
        }
        tmp=-b+disc;
        if(tmp>=0)
        {
            res.t.push_back(tmp);
            return res;
        }
        return res;
    }
};
#endif //RAY_TRACER_SPHERE_HPP
