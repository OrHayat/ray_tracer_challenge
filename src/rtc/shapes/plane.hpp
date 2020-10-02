//
// Created by orhayat on 29/09/2020.
//

#ifndef RAY_TRACER_PLANE_HPP
#define RAY_TRACER_PLANE_HPP
#define GLM_FORCE_SWIZZLE

#include "shape.hpp"
#include <rtc/core/setup_glm.hpp>
struct plane:shape{
    glm::vec4 params;//ax+by+cz+d=0
    glm::vec3 origin;
    glm::vec3 normal;
    explicit plane(glm::vec4 params):params(params){
        this->type=shape_type::t_plane;
        assert(params.x!=0||params.y!=0||params.z!=0);
        this->normal=glm::normalize(params.xyz());
        if(params[0]!=0)
        {
         this->origin=glm::vec3(-params[3]/params[0],0,0);
        }
        else if(params[1]!=0)
        {
            this->origin=glm::vec3(0,-params[3]/params.y,0);
        }
        else//params.[2]!-0
        {
            this->origin=glm::vec3(0,0,-params[3]/params[2]);
        }
    }//initalzing sphere on center of world
    plane(glm::vec3 origin,glm::vec3 normal):params(normal,-glm::dot(origin,normal)),origin(origin),normal(glm::normalize(normal))
    {
        this->type=shape_type::t_plane;
    }
    glm::vec3 get_normal_at_point(glm::vec3 point)const override
    {
        glm::vec4 objectPoint = this->model_inv*glm::vec4(point,1);

        glm::vec4 object_normal=glm::vec4(this->normal,0);

        glm::vec3 worldnormal=(glm::transpose(this->model_inv)*object_normal).xyz;
        return glm::normalize(worldnormal);
    }
    //https://stackoverflow.com/questions/23975555/how-to-do-ray-plane-intersection
    collision_data collide(ray r) override// collision_data collide(ray r)
    {
//        glm::mat4 model_inv=glm::inverse(this->model);
        glm::vec4 new_dir= this->model_inv*glm::vec4(r.dir,0);
        glm::vec4 new_pos= this->model_inv*glm::vec4(r.pos,1);
        r.pos=new_pos;
        r.dir=new_dir;
        collision_data res(*this);
        float denom=glm::dot(glm::normalize(this->normal),glm::normalize(r.dir));
        if(glm::abs(denom)<1e-7)
        {
            return res;
        }
        float nomenitor=r.origin.x*this->params.x+r.origin.y*this->params.y+r.origin.z*this->params.z+this->params.w;  //p0.x*plane.x+p0.y*plane.y+p0.z*plane.z+plane.w;//=glm::dot(plane->params,vec4(r.pos,1))
        res.t.push_back(-nomenitor/denom);
        //https://stackoverflow.com/questions/23975555/how-to-do-ray-plane-intersection
//        res.t.push_back(glm::dot(this->normal,this->origin-r.pos)/denom);
        return res;
    }
};
#endif //RAY_TRACER_PLANE_HPP
