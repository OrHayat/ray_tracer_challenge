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
    int max_depth=3;
    int selected_camera;

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
                bool shadow=false;
                for(unsigned obj_id=0;obj_id<objects.size();++obj_id)
                {
                    collision_data shadow_ray_collision=this->objects.at(obj_id)->collide(shadow_ray);
                    std::optional<float>col_val= shadow_ray_collision.find_collision_value();
//                   if(col_val.has_value())
                    if(col_val.has_value())
                    {
                        if(col_val.value()>0.0005f&&(cur_light->type==light_type::directed_light||col_val.value()<=light_t))
                        {
                            shadow=true;
                            break;
                        }
                    }
                }
                if(!shadow) {
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
                    resulting_color += glm::vec3(intensity.x*kd.x,intensity.y*kd.y,intensity.z*kd.z) * glm::dot(collision_comp.intersection_point_normal, dir_to_lightsource);//diffuse

                    glm::vec3 reflected_light_dir= glm::normalize(glm::reflect(-dir_to_lightsource,collision_comp.intersection_point_normal));
                    float specular_cos_theta=glm::dot(collision_comp.dir_from_intersection_to_eye,reflected_light_dir);
                    resulting_color+=attenuation*glm::vec3(intensity.x*ks.x,intensity.y*ks.y,intensity.z*ks.z)*glm::pow(specular_cos_theta,collision_comp.collided_shape.mat.shininess);
//                glm::vec3 I lights.at(i)->get_lighting_from(intersection_point);
                }
            }
            resulting_color+=glm::vec3(ka.x*I_ambient.x,ka.y*I_ambient.y,ka.z*I_ambient.z);
            if(kr.x>0||kr.y>0||kr.z>0) {
                ray reflection_ray = ray(collision_comp.intersection_point + collision_comp.reflected_ray * 0.0001f,
                                         collision_comp.reflected_ray);
                glm::vec3 tmp = shoot_ray(reflection_ray, depth + 1);
                resulting_color += glm::vec3(kr.x * tmp.x, kr.y * tmp.y, kr.z * tmp.z);
            }
            if(kt.x>0||kt.y>0||kt.z>0)
            {

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
        if(max_id>0) {
            collision_computation comp = collision_computation::prepare_collision_computation(r,
                                                                                              min_t,
                                                                                              collided_shape_data,
                                                                                              collision_result,
                                                                                              max_id,
                                                                                              this->objects);
            //*this->objects.at(max_id));
            return this->shade_hit(comp,depth);
        }
        return  glm::vec3(0,0,0);
    }

#define ANTIALIZE_COUNT 20
    Canvas<glm::vec3> render()
    {
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
