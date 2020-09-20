//
// Created by משתמש1 on 18/09/2020.
//

#include "Camera.hpp"

#include <thread>
#include <iostream>
#include <rtc/core/ray.hpp>
Camera::Camera(unsigned int hSize, unsigned  int vSize, float fov):image_width(hSize),image_height(vSize),fov(fov),camera_to_world_view(glm::mat4())
{
    std::cout<<"camera constructore hSize="<<hSize<<",vSize="<<vSize<<",fov"<<fov<<std::endl;
    float const halfView = glm::tan(fov*0.5f);
    std::cout<<"halfView=:"<<halfView<<std::endl;
    float const aspect = (float)hSize / (float)vSize;
    std::cout<<"aspect=:"<<aspect<<std::endl;
    this->half_width = aspect < 1.f ? halfView * aspect : halfView;
    this->half_height = aspect < 1.f ? halfView : halfView / aspect;
    std::cout<<"half_width="<<half_width<<",half_height="<<half_height<<std::endl;
    this->pixel_size=(float)(this->half_width*2.0f)/(float)this->image_width;
    std::cout<<"pixel_size"<<this->pixel_size<<std::endl;
}

ray Camera::RayForPixel( float x, float y) const
{
    float const xOffset = x*this->pixel_size;//(x + .5f) * this->pixel_size;
    float const yOffset = y*this->pixel_size;//(y + .5f) *this->pixel_size;
    float const cameraX = this->half_height - xOffset;
    float const cameraY = this->half_height - yOffset;
    glm::vec4 const pixel = this->camera_to_world_view * glm::vec4(cameraX, cameraY, -1.0f,1.0f);
    glm::vec4 const origin = camera_to_world_view * glm::vec4(0.f, 0.f, 0.f,1.0f);
    glm::vec4 direction=pixel-origin;
    direction=glm::normalize(direction);

//    if(glm::length(direction)>0.000002f)
//    {
//    }
    return ray(origin, direction);
}