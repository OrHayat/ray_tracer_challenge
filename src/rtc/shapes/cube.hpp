//
// Created by orhayat on 01/10/2020.
//

#ifndef RAY_TRACER_CUBE_HPP
#define RAY_TRACER_CUBE_HPP

#include <rtc/core/setup_glm.hpp>
#include "shape.hpp"
struct cube:shape{
    union {
        glm::vec3 p1;
        glm::vec3 point_min;
    };
    union {
        glm::vec3 p2;
        glm::vec3 point_max;
    };
//    cube():center(glm::vec3(0)),radius(1.0f){}
    explicit cube(glm::vec3 min,glm::vec3 max):point_min(min),point_max(max){
        this->type=shape_type::t_cube;
    }
//    cube(float radius,const glm::vec3& center):center(center),radius(radius){};
//http://blog.johnnovak.net/2016/10/22/the-nim-raytracer-project-part-4-calculating-box-normals/?utm_source=mybridge&utm_medium=blog&utm_campaign=read_more
    glm::vec3 get_normal_at_point(glm::vec3 point,const collision_data& cache)const override
    {
        glm::vec4 objectPoint = this->model_inv*glm::vec4(point,1);
        glm::vec3 new_center=glm::abs(this->point_max- this->point_min)*0.5f;
        glm::vec3 p=(objectPoint.xyz()-new_center);
        int index_max=0;
        if(std::abs(p[1])>std::abs(p[index_max]))
        {
            index_max=1;
        }
        if(std::abs(p[2])>std::abs(p[index_max]))
        {
            index_max=2;
        }
        glm::vec4 object_normal=glm::vec4(0,0,0,1);
        object_normal[index_max]=glm::sign(p[index_max]);
        glm::vec3 worldnormal=(glm::transpose(this->model_inv)*object_normal).xyz;
        return glm::normalize(worldnormal);
    }
    collision_data collide(ray r) override// collision_data collide(ray r)
    {
//        glm::mat4 model_inv=glm::inverse(this->model);
        glm::vec4 new_dir = this->model_inv * glm::vec4(r.dir, 0);
        glm::vec4 new_pos = this->model_inv * glm::vec4(r.pos, 1);
        r.pos = new_pos;
        r.dir = new_dir;

        // r.dir is unit direction vector of ray
        glm::vec3 dirfrac = 1.0f / r.dir;
        // lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
        // r.org is origin of ray
        float t1 = (point_min.x - r.origin.x) * dirfrac.x;
        float t2 = (point_max.x - r.origin.x) * dirfrac.x;
        float t3 = (point_min.y - r.origin.y) * dirfrac.y;
        float t4 = (point_max.y - r.origin.y) * dirfrac.y;
        float t5 = (point_min.z - r.origin.z) * dirfrac.z;
        float t6 = (point_max.z - r.origin.z) * dirfrac.z;

        float tmin = fmax(fmax(fmin(t1, t2), fmin(t3, t4)), fmin(t5, t6));
        float tmax = fmin(fmin(fmax(t1, t2), fmax(t3, t4)), fmax(t5, t6));
        collision_data res(*this);
// if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
        if (tmax < 0 || tmin > tmax) {
            return res;
        }
        if(tmin<0.0001f)
        {
         res.t.push_back(tmax);
            return res;
        }
        else
        {
            res.t.push_back(tmin);
            return res;
        }
    }
};
#endif //RAY_TRACER_CUBE_HPP
