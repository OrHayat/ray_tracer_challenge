//
//

#ifndef RAY_TRACER_SCENE_HPP
#define RAY_TRACER_SCENE_HPP

#define GLM_FORCE_SWIZZLE
#define GLM_FORCE_CTOR_INIT

#include <rtc/core/shape.hpp>
#include <vector>
#include <iostream>
#include <rtc/core/lighting/light.hpp>
#include <rtc/Camera.hpp>
#include <rtc/Canvas.hpp>
struct Scene {
    std::vector<shape*> objects;
    std::vector<Camera>cameras;
    std::vector<light*>lights;
    glm::vec4 I_ambient=glm::vec4(0);
    int max_depth=50;
    int selected_camera;

    glm::vec3 render_block(float x,float y,int block_size_x,int block_size_y,int depth)
    {
        ray r = cameras[selected_camera].RayForPixel(x,y);
        float min_t=-1;
        std::optional<collision_data> collided;
        for(unsigned int i=0;i<objects.size();i++)
        {
            collision_data res=objects.at(i)->collide(r);
            std::optional<float> colision_time=res.find_collision_value();
            float cur_collision_time=colision_time.value_or(-1.0f);
            if(cur_collision_time>=0)
            {
                if((min_t<0)||(cur_collision_time<=min_t))
                {
                    min_t=cur_collision_time;
                    collided=std::make_optional(res);
                }
            }
        }
        if(collided.has_value())
        {
            glm::vec4 ka=collided.value().colided_shape.ka;
            glm::vec4 kd=collided.value().colided_shape.kd;
            glm::vec4 ks=collided.value().colided_shape.ks;
            glm::vec3 intersection_point=r(min_t);
            glm::vec3 normal=glm::normalize(collided.value().colided_shape.get_normal_at_point(intersection_point));
            glm::vec3 resulting_color=glm::vec3(0);
            for(unsigned int i=0;i<lights.size();i++)
            {
                glm::vec3 dir_to_lightsource=(lights.at(i)->pos-intersection_point);
                ray shadow_ray(intersection_point+dir_to_lightsource*0.0005f,dir_to_lightsource);
                bool shadow=false;
                for(unsigned obj_id=0;obj_id<objects.size();++obj_id)
                {
                   collision_data shadow_ray_collision=objects.at(obj_id)->collide(shadow_ray);
                   std::optional<float>col_val= shadow_ray_collision.find_collision_value();
                   if(col_val.has_value()&&col_val.value()<0.9999&&col_val.value()>0)
                   {
                       shadow=true;
                       break;
                   }
                }
                if(!shadow) {
                    dir_to_lightsource=glm::normalize(dir_to_lightsource);
                    glm::vec3 intensity = lights.at(i)->get_lighting_Intensity_from(intersection_point);
                    intensity.x*=kd.x;
                    intensity.y*=kd.y;
                    intensity.z*=kd.z;
                    resulting_color += intensity * glm::dot(normal, dir_to_lightsource);
//                glm::vec3 I lights.at(i)->get_lighting_from(intersection_point);
                }
            }
            resulting_color+=glm::vec3(ka.x*I_ambient.x,ka.y*I_ambient.y,ka.z*I_ambient.z);
            return resulting_color;
        }
        return  glm::vec3(0,0,0);
    }
    Canvas<glm::vec3> render()
    {
        int nonzeros_pixels=0;
        std::cout<<"selected camera is"<<selected_camera<<"out of"<<cameras.size()<<"cameras"<<std::endl;
        Canvas<glm::vec3> res(cameras[selected_camera].image_width,cameras[selected_camera].image_height);
        for(uint32_t x=0;x<cameras[selected_camera].image_width;x++)
        {
            for(uint32_t y=0;y<cameras[selected_camera].image_height;y++)
            {
//                std::cout<<"x="<<x<<" ,y= "<<y<<std::endl;
                glm::vec3 tmp=glm::vec3(0);
                for (unsigned int s = 0; s <4 ; ++s)
                {
                   tmp+=render_block(x+(float)s/2.0f,y+((float)(s%2)/2.0f),1,1,0);
                }
                tmp/=4;
//                if(tmp.x!=0||tmp.y!=0||tmp.z!=0) {
//                    nonzeros_pixels++;
//                    tmp=glm::clamp(tmp*255.0f,glm::vec3(0.0f),glm::vec3(255.0f));
//                    std::cout << "vec3(" << tmp.x << "," << tmp.y << "," << tmp.z << ")" << std::endl;
//                }
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
