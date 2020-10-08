//
// Created by orhayat on 29/09/2020.
//

#ifndef RAY_TRACER_DISK_HPP
#define RAY_TRACER_DISK_HPP
#define GLM_FORCE_SWIZZLE

#include "shape.hpp"
#include <rtc/core/setup_glm.hpp>
struct disk:shape{
    glm::vec4 params;//ax+by+cz+d=0
    glm::vec3 origin;
    glm::vec3 normal;
    float radius;
//    explicit disk(glm::vec4 params,float radius):params(params),radius(radius){
//        assert(params.x!=0||params.y!=0||params.z!=0);
//        this->normal=glm::normalize(params.xyz());
//        if(params[0]!=0)
//        {
//         this->origin=glm::vec3(-params[3]/params[0],0,0);
//        }
//        else if(params[1]!=0)
//        {
//            this->origin=glm::vec3(0,-params[3]/params.y,0);
//        }
//        else//params.[2]!-0
//        {
//            this->origin=glm::vec3(0,0,-params[3]/params[2]);
//        }
//    }//initalzing sphere on center of world
    disk(glm::vec3 origin,glm::vec3 normal,float radius):params(normal,-glm::dot(origin,normal)),origin(origin),normal(glm::normalize(normal)),radius(radius)
    {
        this->type=shape_type::t_disk;
    }
    glm::vec3 get_normal_at_point(glm::vec3 point,const collision_data& cache)const override
    {
        glm::vec4 objectPoint = this->model_inv*glm::vec4(point,1);

        glm::vec4 object_normal=glm::vec4(this->normal,0);

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
        collision_data res(*this);
        float denom=glm::dot(this->normal,glm::normalize(r.dir));
        if(glm::abs(denom)<1e-7)
        {
            return res;
        }
        //float nomenitor= p0.x*plane.x+p0.y*plane.y+p0.z*plane.z+plane.w;//=glm::dot(plane->params,vec4(r.pos,1))
        //res.t.push_back(nom/denom);
        //https://stackoverflow.com/questions/23975555/how-to-do-ray-plane-intersection
        float t=glm::dot(this->normal,this->origin-r.pos)/denom;
        glm::vec3 v= r(t)- this->origin;
        if(glm::dot(v,v)<= this->radius*this->radius)
        {
        res.t.push_back(t);
        }
        return res;
    }
};
#endif //RAY_TRACER_DISK_HPP
