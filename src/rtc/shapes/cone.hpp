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
    glm::vec3 pa;
    glm::vec3 pb;
//    glm::vec3 p_apix;
    float radius_a;
    float radius_b;
//    float radius;
    glm::vec3 cone_axis;
//    bool infiite;
    cone():pa(glm::vec3(0,1,0)),pb(glm::vec3(0)),radius_a(0.5),radius_b(0.2){//:cos_theta(0.955),apex_point(glm::vec3(0,1,1)),min_height(0),max_height(0),cone_axis(glm::vec3(0,1,0)),is_infinite(true){
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
    glm::vec3 get_normal_at_point(glm::vec3 point,const collision_data& cache)const override
    {
        glm::vec4 objectPoint = this->model_inv*glm::vec4(point,1);
        glm::vec3 object_normal=glm::vec3(cache.cache[0],cache.cache[1],cache.cache[2]);
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


        collision_data res(*this);
        glm::vec3 ba = this->pb - this->pa;
        glm::vec3 oa = r.origin - this->pa;
        glm::vec3 ob = r.origin - this->pb;

        float m0 = glm::dot(ba,ba);
        float m1 = glm::dot(oa,ba);
        float m2 = glm::dot(ob,ba);
        float m3 = glm::dot(r.dir,ba);

        //caps
//        if( m1<0.0 ) { if( dot2(oa*m3-rd*m1)<(ra*ra*m3*m3) ) return vec4(-m1/m3,-ba*inversesqrt(m0)); }
//        else if( m2>0.0 ) { if( dot2(ob*m3-rd*m2)<(rb*rb*m3*m3) ) return vec4(-m2/m3, ba*inversesqrt(m0)); }

        // body
        float m4 = glm::dot(r.dir,oa);
        float m5 = glm::dot(oa,oa);
        float rr = this->radius_a - this->radius_b;
        float hy = m0 + rr*rr;

        float k2 = m0*m0    - m3*m3*hy;
        float k1 = m0*m0*m4 - m1*m3*hy + m0*this->radius_a*(rr*m3*1.0);
        float k0 = m0*m0*m5 - m1*m1*hy + m0*this->radius_a*(rr*m1*2.0 - m0*this->radius_a);

        float disc = k1*k1 - k2*k0;
        if( disc<0.0 ) return res;
        float delta=glm::sqrt(disc);
        float t1 = (-k1-delta)/k2;
        float t2=(-k1+delta)/k2;
        if(t1>0)
        {
            float t=t1;
            float y = m1 + t*m3;
            if( y>0.0 && y<m0) {
                res.t.push_back(t1);
                glm::vec3 n=glm::normalize(m0*(m0*(oa+t*r.dir)+rr*ba*radius_a)-ba*hy*y);
                res.cache[0]=n.x;
                res.cache[1]=n.y;
                res.cache[2]=n.z;
                res.t.push_back(t2);
                return res;
            }
            t=t2;
            y = m1 + t*m3;
            if( y>0.0 && y<m0)
            {
                res.t.push_back(t2);
                glm::vec3 n=glm::normalize(m0*(m0*(oa+t*r.dir)+rr*ba*radius_a)-ba*hy*y);
                res.cache[0]=n.x;
                res.cache[1]=n.y;
                res.cache[2]=n.z;
                return res;
            }
        }
        if(t2>0)
        {
            float t=t2;
            float y = m1 + t*m3;
            if( y>0.0 && y<m0) {
                res.t.push_back(t2);
                glm::vec3 n = glm::normalize(m0 * (m0 * (oa + t * r.dir) + rr * ba * radius_a) - ba * hy * y);
                res.cache[0] = n.x;
                res.cache[1] = n.y;
                res.cache[2] = n.z;
                return res;
            }
        }
        return res;
    }
};
#endif //RAY_TRACER_CONE_HPP
