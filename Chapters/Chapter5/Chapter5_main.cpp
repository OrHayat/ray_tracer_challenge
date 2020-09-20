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
//#include <glm/gtx/transform2.hpp>
int main(int argc, char** argv)
{
    Scene s;
    Camera cam(650,400,glm::pi<float>()/2);
    cam.camera_to_world_view=glm::translate(glm::mat4(),glm::vec3(0,0,5));
    s.I_ambient=glm::vec4(1.0f,1.0f,1.0f,1.0f);
    s.selected_camera=0;
    s.cameras.push_back(cam);
    sphere* sphere_= new sphere();
    sphere_->ka=glm::vec4(1.0f,1.0,0,1.0f);
    s.objects.push_back(sphere_);
    Canvas<glm::vec3> can= s.render();
    Canvas<glm::u8vec3> res(can.width,can.height);
//    res.data=(glm::u8vec3 *)(can.data);
    for (int i = 0; i <res.width*res.height ; ++i) {
        res.data[i]=can.data[i];
    }
    int x=stbi_write_bmp("./output1234.bmp",res.width,res.height,3,res.data);
    printf("x=%d\n",x);

    return 0;
}