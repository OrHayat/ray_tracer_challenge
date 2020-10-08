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
#include <rtc/shapes/sphere.hpp>
#include <rtc/shapes/plane.hpp>
#include <rtc/shapes/cube.hpp>
#include <rtc/shapes/cone.hpp>

#include <rtc/shapes/disk.hpp>
int main(int argc, char** argv)
{
    Scene s;
    Camera cam(650,400,glm::pi<float>()/2);
    cam.set_transform(glm::lookAt(glm::vec3(1,4,4),glm::vec3(0),glm::vec3(0,1,0)));
    //cam.set_transform(glm::scale(glm::translate(glm::mat4(),glm::vec3(0,0,-4)),glm::vec3(-1,-1,1)));
    s.I_ambient=glm::vec4(0.6f,0.7f,0.7f,1.0f);
    s.selected_camera=0;
    s.cameras.push_back(cam);
    sphere* sphere_= new sphere(1.5,glm::vec3(-1.0f));
    sphere* sphere_2= new sphere();

//    disk* disk_=new plane(glm::vec3(0),glm::vec3(0,1,0));
    plane* disk_=new plane(glm::vec3(-5.0f,-5.0f,-3.0f),glm::vec3(0,1.0f,0));//,2);
//    cube* c=new cube(glm::vec3(0,0,0),glm::vec3(0.5f));
    cone* c=new cone();
    material mat_;
    mat_.ka=glm::vec4(0.15f,0.35f,0.7f,1.0f);
    mat_.kd=glm::vec4(0.35f,0.85f,0.7f,1.0f);
    mat_.ks=glm::vec4(1.0f,0.5,1,1);
    mat_.shininess=250.0f;
    sphere_->mat=mat_;
    disk_->mat=mat_;
    c->mat=mat_;
    c->mat.ka=glm::vec4(0.2,0.3,1.0f,1.0f);
    disk_->mat.ka=glm::vec4(0.0f,0,1.0f,1.0f);
    disk_->mat.kd=glm::vec4(0,1,1,1);
    disk_->mat.ks=glm::vec4(0,0.5,0.72,1);
    disk_->set_model(glm::mat4(1.0f));
    sphere_->set_model(glm::mat4(1.0f));
    sphere_2->set_model(glm::translate(glm::mat4(),glm::vec3(1,1,-4)));
    sphere_2->mat.ka=glm::vec4(1.0f,0,0,1.0f);
    sphere_2->mat.kd=glm::vec4(1.0f);

//    sphere_->set_model((glm::translate(glm::mat4(),glm::vec3(0,0,0))*glm::scale(glm::mat4(),glm::vec3(1,1,1))));
//    sphere_->model=(glm::translate(glm::mat4(),glm::vec3(2,0,0))*glm::scale(glm::mat4(),glm::vec3(3,3,3)));
//    sphere_->model_inv=glm::inverse(sphere_->model);
    s.objects.push_back(sphere_);
    s.objects.push_back(disk_);
    s.objects.push_back(c);
    s.objects.push_back(sphere_2);
    glm::vec3 light1_pos=glm::vec3(0,5.0f,4.0f);
    light* point_light1=light::make_point_light_ptr(light1_pos);
    point_light1->kc=1.2f;
    point_light1->kq=0.002f;
    point_light1->intensity=glm::vec4(1.0f,1.0f,1.0f,1.0f);
    glm::vec3 light2_pos=glm::vec3(5.0f,-6.0f,-5.0f);
    light* point_light2=light::make_point_light_ptr(light2_pos);
    point_light2->intensity=glm::vec4(1.0f,1.0f,1.0f,1.0f);
    point_light2->kc=1.0f;
    light* directional_light=light::make_directed_light_ptr(glm::vec3(1,0,0),glm::vec4(0,0,1,1));
    s.lights.push_back(point_light1);
    s.lights.push_back(directional_light);
    Canvas<glm::vec3> can= s.render();
//    can.drawline(glm::ivec2(200,20),glm::ivec2(200,10),glm::vec3(0,255.0f,0));
    can.set_pixel(330,230,glm::vec3(0,255.0f,0));
    Canvas<glm::u8vec3> res(can.width,can.height);
    for (int i = 0; i <res.width*res.height ; ++i)
    {
        res.data[i]=can.data[i];
    }
//    res.draw_circle(glm::ivec2(450,350),30,glm::u8vec3(0,255,0),6);
    int x=stbi_write_bmp("./chapter9.bmp",res.width,res.height,3,res.data);
    printf("x=%d\n",x);

    return 0;
}