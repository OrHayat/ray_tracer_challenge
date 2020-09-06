//
// Created by משתמש1 on 01/09/2020.
//
#define GLM_FORCE_SWIZZLE
#define GLM_FORCE_CTOR_INIT


#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <shared/env.hpp>
#include <shared/projectile.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <rtc/Canvas.hpp>
#include <stb_image_write.h>
#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtx/transform2.hpp>
int main(int argc, char** argv)
{
    Canvas<glm::u8vec3>canvas(900,900);
    glm::vec3 center(100.0f,100.0f,100.0f);
    glm::mat4 rot;


    for(int i=0;i<16;i++)
    {
        glm::mat4 Model = glm::translate(glm::mat4(),center);
        rot=glm::rotate(rot,360.0f/16.0f,glm::vec3(0,0,1));
        Model=rot*Model;
//        rot=glm::rotate(rot,glm::radians(i*(360.0f/16.0f)),glm::vec3(0,0,1));//glm::radians(360.0f/i),glm::vec3(0,0,1));
        std::cout<<std::endl;



        glm::vec4 m = Model*glm::vec4(0,0,1,1);
       std::cout<<"m=["<<m.x<<","<<m.y<<","<<m.z<<","<<m.w<<"]"<<std::endl;
       canvas.set_pixel(glm::u8vec3(255,255,255),(int)m.x,(int)m.y);
    }
    int x=stbi_write_bmp("./output123.bmp",canvas.width,canvas.height,3,canvas.data);
//    int x=stbi_write_jpg("./outputtest111.jpg:",canvas.width,canvas.height,3,canvas.data,900);
    std::cout<<"foo";
    return 0;
}