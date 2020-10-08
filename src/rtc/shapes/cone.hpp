//
// Created by orhayat on 07/10/2020.
//

#ifndef RAY_TRACER_CONE_HPP
#define RAY_TRACER_CONE_HPP

#define GLM_FORCE_SWIZZLE
#include <rtc/shapes/shape.hpp>
#include <rtc/core/ray.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

struct cone:shape{
//    float theta;
    float cos_theta;
    glm::vec3 apex_point;
    float max_height;
    float min_height;
    bool is_infinite;
//    glm::vec3 p1;
//    glm::vec3 p2;
//    float radius;
    glm::vec3 cone_axis;
//    bool infiite;
    cone():cos_theta(0.95),apex_point(glm::vec3(0,2,0)),min_height(0),max_height(1),is_infinite(false){
//        this->cylinder_axis=glm::normalize(p2-p1);
        this->type=shape_type::t_cone;
    }
//    explicit sphere(float radius):center(glm::vec3(0)),radius(radius)
//    {
//        this->type=shape_type::t_sphere;
//    }//initalzing sphere on center of world
//    sphere(float radius,const glm::vec3& center):center(center),radius(radius)
//    {
//        this->type=shape_type::t_sphere;
//    };

//https://www.shadertoy.com/view/MtcXWr
    glm::vec3 get_normal_at_point(glm::vec3 point)const override
    {
        glm::vec4 objectPoint = this->model_inv*glm::vec4(point,1);
        glm::vec3 cp = point-this->apex_point;
        float hs = glm::dot(cp,this->cone_axis);
        if(hs<0)
        {
            hs=-1.0f;
        }
        else
        {
            hs=1.0f;
        }
        glm::vec3 object_normal =hs*glm::normalize(cp * glm::dot(this->cone_axis, cp) / dot(cp, cp) - this->cone_axis);
        glm::vec3 worldnormal=(glm::transpose(this->model_inv)*glm::vec4(object_normal,0)).xyz;
        return glm::normalize(worldnormal);
    }

    //https://lousodrome.net/blog/light/2017/01/03/intersection-of-a-ray-and-a-cone/

    collision_data collide(ray r) override// collision_data collide(ray r)
    {
//        glm::mat4 model_inv=glm::inverse(this->model);
        glm::vec4 new_dir= this->model_inv*glm::vec4(r.dir,0);
        glm::vec4 new_pos= this->model_inv*glm::vec4(r.pos,1);
        r.pos=new_pos;
        r.dir=glm::normalize(new_dir);
//        r=ray(new_pos,new_dir);
        //x1x,x=(-b(+,-)sqrt(b^2-4ac))/2a when dot(a,a)=1 and b=2*dot(r.dir,sphere to origin) then=x1,x2=-b(+,-)sqrt(b^2-c)

        glm::vec3 co = r.origin - this->apex_point;
        float costheta_sq=this->cos_theta*this->cos_theta;
        float a = glm::pow(dot(r.dir,this->cone_axis),2)  -costheta_sq;;;
        float b = 2.0f * (glm::dot(r.dir,this->cone_axis)*dot(co,this->cone_axis) - glm::dot(r.dir,co)*this->cos_theta*costheta_sq);
        float c = glm::pow(glm::dot(co,this->cone_axis),2) - glm::dot(co,co)*costheta_sq;

        collision_data res(*this);
        float disc=b*b-a*c;
        if(disc<0)
        {
            return res;
        }
        disc=std::sqrt(disc);
        float t1=(-b-disc)/a;
        float t2=(-b+disc)/a;
        if(t1>0) {
            glm::vec3 p=r(t1);
            glm::vec3 cp = p-this->apex_point;
            float h = glm::dot(cp,this->cone_axis);
            if(h>min_height&&h<=max_height) {
                res.t.push_back(t1);
            }
            p=r(t2);
            cp = p-this->apex_point;
            h = glm::dot(cp,this->cone_axis);
            if(h>min_height&&h<=max_height) {
                res.t.push_back(t2);
            }
            return res;
        }
        if(t2>0)
        {
        glm::vec3 p=r(t2);
        glm::vec3 cp = p-this->apex_point;
        float h = glm::dot(cp,this->cone_axis);
        if(h>min_height&&h<=max_height) {
            res.t.push_back(t2);
        }
            return res;
        }
        return res;
    }
};
#endif //RAY_TRACER_CONE_HPP
