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
    canvas.drawline(glm::ivec2(center.x,center.y),glm::ivec2(160,200),glm::vec3(255,0,0));
    canvas.draw_circle(glm::ivec2(250,260),25,glm::vec3(0,0,255));
    int x=stbi_write_bmp("./output123.bmp",canvas.width,canvas.height,3,canvas.data);
//    int x=stbi_write_jpg("./outputtest111.jpg:",canvas.width,canvas.height,3,canvas.data,900);
    std::cout<<"foo";
    return 0;
}