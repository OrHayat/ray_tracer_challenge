//
//

#ifndef RAY_TRACER_SCENE_HPP
#define RAY_TRACER_SCENE_HPP

#define GLM_FORCE_SWIZZLE
#define GLM_FORCE_CTOR_INIT

#include <rtc/shapes/shape.hpp>
#include <vector>
#include <iostream>
#include <rtc/core/lighting/light.hpp>
#include <rtc/Camera.hpp>
#include <rtc/Canvas.hpp>
#include <rtc/core/setup_glm.hpp>
//#include <rtc/core/ray.hpp>
#include "algorithm"

struct collision_with_scene_result_data
{
    collision_data col_data_cpy;
    float t;
    shape * collided_shape;
    collision_with_scene_result_data(float t, shape* s,collision_data col_data_cpy):t(t),collided_shape(s),col_data_cpy(col_data_cpy){};
};
struct collision_with_scene_result
{
std::vector<collision_with_scene_result_data>data;
    collision_with_scene_result_data operator [](int i){return data[i];}
};


struct Scene {
        collision_with_scene_result scene_ray_collide(ray r){
        collision_with_scene_result res;
        for(unsigned int i=0;i< this->objects.size();i++)
        {
            collision_data d= this->objects.at(i)->collide(r);
            for(unsigned int j=0;j<d.t.size();++j)
            {
                collision_with_scene_result_data cur_data(d.t[j],d.colided_shape,d);
                res.data.push_back(cur_data);
            }
        }
        std::sort(res.data.begin(),res.data.end(),[](collision_with_scene_result_data a,collision_with_scene_result_data b){
            return a.t<b.t;
        });
        return res;
    };
    std::vector<shape*> objects;
    std::vector<Camera>cameras;
    std::vector<light*>lights;
    glm::vec4 I_ambient=glm::vec4(0);
    int max_depth=20;
    int selected_camera;

    float Schlick(collision_computation comp)
    {
        float cos=glm::dot(comp.dir_from_intersection_to_eye,comp.intersection_point_normal);
            // cosine of the angle between the eye and normal vectors
            // total internal reflection can only ocurr if n1 > n2
            if (comp.n1>comp.n2)
            {
                float const n = comp.n1/comp.n2;
                float const sin2T = n * n * (1.0f - (cos * cos));
                if (sin2T > 1.f)
                {
                    return 1.f;
                }
                // compute cosine of theta using trig identity
                float const cosT = std::sqrtf(1.f - sin2T);
                // when n1 > n2 use cosine of theta instead
                cos = cosT;
            }

            float const r0 = std::powf((comp.n1-comp.n2) / (comp.n1+comp.n2), 2.f);
            return r0 + ((1.0f - r0) * std::powf(1.f - cos, 5.f));
        }
    glm::vec3 shade_hit(const collision_computation& collision_comp,float depth)
    {
            glm::vec4 ka=collision_comp.collided_shape.mat.ka;
            glm::vec4 kd=collision_comp.collided_shape.mat.kd;
            glm::vec4 ks=collision_comp.collided_shape.mat.ks;
            glm::vec4 kr=collision_comp.collided_shape.mat.kr;
            glm::vec4 kt=collision_comp.collided_shape.mat.kt;

//            std::cout<<"ka="<<glm::to_string(ka)<<std::endl;
//            std::cout<<"kd="<<glm::to_string(kd)<<std::endl;
//            std::cout<<"ks="<<glm::to_string(ks)<<std::endl;
//           std::cout<<"I_ambient of scene="<<glm::to_string(this->I_ambient)<<std::endl;


//            glm::vec3 intersection_point=collision_compray_from_eye(collision_comp.t);
//            glm::vec3 normal=glm::normalize(collided.value().colided_shape.get_normal_at_point(collision_comp.intersection_point));
//            if(glm::dot(normal,-ray_from_eye.dir)<0)
//            {
//                normal=-normal;
//            }
            glm::vec3 resulting_color=glm::vec3(0);
            for(unsigned int i=0;i<lights.size();i++)
            {
                light* cur_light= this->lights.at(i);
                glm::vec3 dir_to_lightsource;
                if(cur_light->type==light_type::directed_light)
                {
                    dir_to_lightsource=-cur_light->dir;
                }
                else {//point light or spotlight
                    dir_to_lightsource=(cur_light->pos - collision_comp.intersection_point);
//                    std::cout<<"cur lightsource pos is:"<<glm::to_string(cur_light->pos)<<",,,, and intersection point is: "<<glm::to_string(intersection_point)<<std::endl;
                }
                if(cur_light->type==light_type::spot_light)
                {

                    float cos_theta=glm::dot(glm::normalize(dir_to_lightsource),collision_comp.intersection_point_normal);
                    if(cos_theta>cur_light->cutoff_angel)//the angle is between 0 and 180 degress so for x1,x2 if x2>x1 cos(x1)>cos(x2)
                    {
                        continue;
                    }
                }
                float light_t=glm::length(dir_to_lightsource);
                dir_to_lightsource=dir_to_lightsource/light_t;
                ray shadow_ray(collision_comp.intersection_point+collision_comp.intersection_point_normal*0.005f,dir_to_lightsource);
                collision_with_scene_result shadow_ray_collision=scene_ray_collide(shadow_ray);
                glm::vec4 shadow=glm::vec4(1.0f);
                for(unsigned intersection_id=0;intersection_id<shadow_ray_collision.data.size();++intersection_id)
                {
//                    if(shadow_ray_collision.data.at(intersection_id).collided_shape->id==collision_comp.collided_shape.id)
//                    {
//                        continue;
//                    }
                        float col_val=shadow_ray_collision.data.at(intersection_id).t;
                    {
                        if(col_val>0.0005f&&(cur_light->type==light_type::directed_light||col_val<=light_t))
                        {
                            glm::vec4 tmp=shadow_ray_collision.data.at(intersection_id).collided_shape->mat.kt;
                            shadow.x*=tmp.x;
                            shadow.y*=tmp.y;
                            shadow.z*=tmp.z;
//                            shadow=true;
                        }
                    }
                }
                if(true) {
                    dir_to_lightsource=glm::normalize(dir_to_lightsource);

                    glm::vec3 intensity;
                    float attenuation=1.0f;
                    switch (cur_light->type) {
                        case light_type::point_light:
                        {
                            float dist=glm::distance(cur_light->pos,collision_comp.intersection_point);
                            attenuation=1/(((cur_light->kq*dist)+cur_light->kl)*dist+cur_light->kc);
                            intensity=cur_light->intensity*attenuation;
                            break;
                        }
                        case light_type::directed_light:
                        {
                            intensity=cur_light->intensity*glm::dot(collision_comp.intersection_point_normal,-cur_light->dir);
                            break;
                        }
                        case light_type::spot_light:
                        {
                            float dist=glm::distance(cur_light->pos,collision_comp.intersection_point);
                            attenuation=1/(((cur_light->kq*dist)+cur_light->kl)*dist+cur_light->kc);
                            intensity=cur_light->intensity*glm::dot(collision_comp.intersection_point_normal,-cur_light->dir)*attenuation;
                            break;
                        }
                    }
                    resulting_color += glm::vec3(shadow.x*intensity.x*kd.x,shadow.y*intensity.y*kd.y,shadow.z*intensity.z*kd.z) * glm::dot(collision_comp.intersection_point_normal, dir_to_lightsource);//diffuse
                    glm::vec3 reflected_light_dir= glm::normalize(glm::reflect(-dir_to_lightsource,collision_comp.intersection_point_normal));
                    float specular_cos_theta=glm::dot(collision_comp.dir_from_intersection_to_eye,reflected_light_dir);
                    resulting_color+=attenuation*glm::vec3(shadow.x*intensity.x*ks.x,shadow.y*intensity.y*ks.y,shadow.z*intensity.z*ks.z)*glm::pow(specular_cos_theta,collision_comp.collided_shape.mat.shininess);
//                glm::vec3 I lights.at(i)->get_lighting_from(intersection_point);
                }
            }

            resulting_color+=glm::vec3(ka.x*I_ambient.x,ka.y*I_ambient.y,ka.z*I_ambient.z);
            float reflctance=1.0f;
            if((kr.x>0||kr.y>0||kr.z>0)&&(kt.x>0||kt.y>0||kt.z>0))
            {
                reflctance=Schlick(collision_comp);
            }
            if(kr.x>0||kr.y>0||kr.z>0) {
                ray reflection_ray = ray(collision_comp.intersection_point + collision_comp.reflected_ray * 0.0001f,
                                         collision_comp.reflected_ray);
                glm::vec3 tmp = shoot_ray(reflection_ray, depth + 1);
                resulting_color += reflctance*glm::vec3(kr.x * tmp.x, kr.y * tmp.y, kr.z * tmp.z);
            }
            if(kt.x>0||kt.y>0||kt.z>0)
            {
                float const nRatio = collision_comp.n1/collision_comp.n2;
                float const cosI =glm::dot(collision_comp.intersection_point,collision_comp.intersection_point_normal);
                float const sin2T = (nRatio * nRatio) * (1.0f - (cosI * cosI));
                if (!(sin2T > 1.0f))
                {
                    float const cosT = glm::sqrt(1.0f - sin2T);
                    glm::vec3 const direction = (collision_comp.intersection_point_normal * ((nRatio * cosI) - cosT)) - (collision_comp.dir_from_intersection_to_eye * nRatio);
                    ray  refractedRay(collision_comp.intersection_point+direction*0.0001f, direction);
                    glm::vec3 tmp = shoot_ray(refractedRay, depth + 1);
                    resulting_color += reflctance*glm::vec3(kt.x * tmp.x, kt.y * tmp.y, kt.z * tmp.z);
                }
            }
            return resulting_color;
        }
    glm::vec3 render_block(float x,float y,int block_size_x,int block_size_y,int depth)
    {
        if(depth>=this->max_depth)
        {
            return glm::vec3(0);
        }
        ray ray_from_eye = cameras[selected_camera].RayForPixel(x,y);
        return shoot_ray(ray_from_eye,0);
    }
    glm::vec3 shoot_ray(ray & r,float depth)
    {
        float min_t=-1.0f;
        collision_data collided_shape_data;
        int max_id=-1;
        collision_with_scene_result collision_result= scene_ray_collide(r);
        for(unsigned int i=0;i<collision_result.data.size();i++)
        {
            float cur_collision_time=collision_result.data.at(i).t;
            if((min_t<0)||(cur_collision_time<=min_t))
            {
                max_id=i;
                min_t=cur_collision_time;
                collided_shape_data=collision_result.data.at(i).col_data_cpy;
            }
        }
        /*
        for(unsigned int i=0;i<objects.size();i++)
        {
            collision_data res=objects.at(i)->collide(r);
            std::optional<float> collision_time=res.find_collision_value();
            float cur_collision_time=collision_time.value_or(-10000.0f);
            if(cur_collision_time>0)
            {
                if((min_t<0)||(cur_collision_time<=min_t))
                {
                    max_id=i;
                    min_t=cur_collision_time;
                    collided=res;
                }
            }
        }*/
        if(min_t>0.0005f) {
            collision_computation comp = collision_computation::prepare_collision_computation(r,
                                                                                              min_t,
                                                                                              collided_shape_data,
                                                                                              max_id,
                                                                                              collision_result,
                                                                                              this->objects
                                                                                              );
            //*this->objects.at(max_id));
            return this->shade_hit(comp,depth);
        }
        return  glm::vec3(0,0,0);
    }

#define ANTIALIZE_COUNT 20
    Canvas<glm::vec3> render()
    {
        for(unsigned int i=0;i<objects.size();i++)
        {
            objects.at(i)->id=i;
        }
        int nonzeros_pixels=0;
//        std::cout<<"selected camera is"<<selected_camera<<"out of"<<cameras.size()<<"cameras"<<std::endl;
        Canvas<glm::vec3> res(cameras[selected_camera].image_width,cameras[selected_camera].image_height);
        for(uint32_t x=0;x<cameras[selected_camera].image_width;x++)
        {
            for(uint32_t y=0;y<cameras[selected_camera].image_height;y++)
            {
//                std::cout<<"x="<<x<<" ,y= "<<y<<std::endl;
                glm::vec3 tmp(0);
#ifdef USE_AA
                for (unsigned int s = 0; s <ANTIALIZE_COUNT*ANTIALIZE_COUNT ; ++s)
                {
                   tmp+=render_block(x+(float)s/(float)ANTIALIZE_COUNT,y+((float)(s%ANTIALIZE_COUNT)/(float)ANTIALIZE_COUNT),1,1,0);
                }
                tmp/=(ANTIALIZE_COUNT*ANTIALIZE_COUNT);
#else
            tmp += render_block(x + (float) 0.5f, y + 0.5f, 1, 1, 0);
#endif
#undef ANTIALIZE_COUNT
#undef USE_AA
                //        if(x==330&&y==230)
//        {
//            tmp += render_block(x + (float) 0.5f, y + 0.5f, 1, 1, 1);
//        }
//        else {
//        }
                res.set_pixel(x,y,glm::clamp(tmp*255.0f,glm::vec3(0.0f),glm::vec3(255.0f)));
            }
        }
//        res.draw_circle(glm::ivec2(50,50),14,glm::vec3(255.0f,0,0));
        std::cout<<"nonzero_pixels"<<nonzeros_pixels<<std::endl;

        return res;
    }
//     Options &options,
//     std::vector<> &lights)


};


#endif //RAY_TRACER_SCENE_HPP
