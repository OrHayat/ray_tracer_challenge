//
// Created by orhayat on 05/10/2020.
//

#ifndef RAY_TRACER_CYLINDER_HPP
#define RAY_TRACER_CYLINDER_HPP

#define GLM_FORCE_SWIZZLE

#include <rtc/shapes/shape.hpp>
#include <rtc/core/ray.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>


struct cylinder:shape{
    glm::vec3 p1;
    glm::vec3 p2;
    float radius;
    glm::vec3 cylinder_axis;
    cylinder():p1(glm::vec3(0)),p2(glm::vec3(0,1,0)),radius(1.0f),cylinder_axis(glm::vec3(0,1,0)){
//        this->cylinder_axis=glm::normalize(p2-p1);
        this->type=shape_type::t_cylinder;
    }
//    explicit sphere(float radius):center(glm::vec3(0)),radius(radius)
//    {
//        this->type=shape_type::t_sphere;
//    }//initalzing sphere on center of world
//    sphere(float radius,const glm::vec3& center):center(center),radius(radius)
//    {
//        this->type=shape_type::t_sphere;
//    };
    glm::vec3 get_normal_at_point(glm::vec3 point)const override
    {
        glm::vec4 objectPoint = this->model_inv*glm::vec4(point,1);
        glm::vec3 object_normal=/*2*?*/(point- this->p1-glm::dot(this->cylinder_axis,point-this->p1)*this->cylinder_axis);
        glm::vec3 worldnormal=(glm::transpose(this->model_inv)*glm::vec4(object_normal,0)).xyz;
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
        glm::vec3 tmp=r.dir-glm::dot(r.dir,this->cylinder_axis)* this->cylinder_axis;
        float a=glm::dot(tmp,tmp);
        glm::vec3 delta_p=r.origin-this->p1;
        glm::vec3 tmp2=delta_p-glm::dot(delta_p,this->cylinder_axis)*this->cylinder_axis;
        float b=glm::dot(tmp,tmp2);
        float c=glm::dot(tmp2,tmp2)-this->radius*this->radius;
        collision_data res(*this);
        float disc=b*b-a*c;
        if(disc<0)
        {
            return res;
        }
        disc=std::sqrt(disc);
        res.t.push_back((-b-disc)/a);
        res.t.push_back((-b+disc)/a);
        return res;
    }
};

#endif //RAY_TRACER_CYLINDER_HPP
