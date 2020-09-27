//
// Created by משתמש1 on 18/09/2020.
//

#include "Camera.hpp"

#include <thread>
#include <iostream>
#include <rtc/core/ray.hpp>
#include <glm/gtx/string_cast.hpp>
Camera::Camera(unsigned int hSize, unsigned  int vSize, float fov):image_width(hSize),image_height(vSize),fov(fov),world_to_camera_view(glm::mat4(1.0f)),camera_to_world_view(glm::mat4(1.0f))
{
    std::cout<<glm::to_string(world_to_camera_view)<<std::endl;
    float const halfView = glm::tan(fov*0.5f);
    std::cout<<"halfView=:"<<halfView<<std::endl;
    float const aspect = (float)hSize / (float)vSize;
    std::cout<<"aspect=:"<<aspect<<std::endl;
    this->half_width = aspect < 1.f ? halfView * aspect : halfView;
    this->half_height = aspect < 1.f ? halfView : halfView / aspect;
    std::cout<<"half_width="<<half_width<<",half_height="<<half_height<<std::endl;
    this->pixel_size=(float)(this->half_width*2.0f)/((float)(this->image_width));
    std::cout<<"pixel_size"<<this->pixel_size<<std::endl;
}


ray Camera::RayForPixel( float x, float y) const
{
    float const xOffset = x*this->pixel_size;//(x + .5f) * this->pixel_size;
    float const yOffset = y*this->pixel_size;//(y + .5f) *this->pixel_size;
//    std::cout<<"x-offset===== "<<xOffset<<",y-offset==== "<<yOffset<<std::endl;
    float const cameraX = this->half_width - xOffset;
    float const cameraY = this->half_height - yOffset;
    glm::vec4 const pixel = this->world_to_camera_view * glm::vec4(cameraX, cameraY, -1.0f,1.0f);
    glm::vec4 const origin = world_to_camera_view * glm::vec4(0.f, 0.f, 0.f,1.0f);
    glm::vec4 direction=pixel-origin;
    direction=glm::normalize(direction);

//    if(glm::length(direction)>0.000002f)
//    {
//    }
    return ray(origin, direction);
}