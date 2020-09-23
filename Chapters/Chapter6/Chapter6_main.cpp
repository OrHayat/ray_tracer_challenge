//
//
#define GLM_FORCE_SWIZZLE
#define GLM_FORCE_CTOR_INIT

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <shared/env.hpp>
#include <shared/projectile.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <rtc/Canvas.hpp>
#include <rtc/Scene.hpp>
#include <rtc/Camera.hpp>
#include <stb_image_write.h>
#include <glm/gtc/matrix_transform.hpp>
#include <rtc/core/sphere.hpp>
#include <rtc/core/lighting/point_light.hpp>
int main(int argc, char** argv)
{
    Scene s;
    Camera cam(650,400,glm::pi<float>()/2);
    cam.camera_to_world_view=glm::scale(glm::translate(glm::mat4(),glm::vec3(0,0,4)),glm::vec3(-1,-1,1));
    s.I_ambient=glm::vec4(0.6f,0.4f,0.5f,1.0f);
    s.selected_camera=0;
    s.cameras.push_back(cam);
    sphere* sphere_= new sphere();
    sphere_->ka=glm::vec4(0.85f,0.35f,0.2f,1.0f);
    sphere_->kd=glm::vec4(1.0f,0.35f,0.5f,1.0f);
    sphere_->set_model(glm::mat4());
//    sphere_->set_model((glm::translate(glm::mat4(),glm::vec3(0,0,0))*glm::scale(glm::mat4(),glm::vec3(1,1,1))));
//    sphere_->model=(glm::translate(glm::mat4(),glm::vec3(2,0,0))*glm::scale(glm::mat4(),glm::vec3(3,3,3)));
//    sphere_->model_inv=glm::inverse(sphere_->model);
    s.objects.push_back(sphere_);
    point_light* point_light1=new point_light();
    point_light1->kc=1.2f;
    point_light1->pos=glm::vec3(0.0f,2.0f,4.0f);
    point_light1->intensity=glm::vec3(1.0f,1.0f,1.0f);
    point_light* point_light2=new point_light();
    point_light2->pos=glm::vec3(5.0f,-6.0f,-5.0f);
    point_light2->intensity=glm::vec3(1.0f,1.0f,1.0f);
    point_light2->kc=1.0f;

    s.lights.push_back(point_light1);
    s.lights.push_back(point_light2);
    Canvas<glm::vec3> can= s.render();
    Canvas<glm::u8vec3> res(can.width,can.height);
    for (int i = 0; i <res.width*res.height ; ++i)
    {
        res.data[i]=can.data[i];
    }
//    res.draw_circle(glm::ivec2(450,350),30,glm::u8vec3(0,255,0),6);
    int x=stbi_write_bmp("./output1234.bmp",res.width,res.height,3,res.data);
    printf("x=%d\n",x);

    return 0;
}