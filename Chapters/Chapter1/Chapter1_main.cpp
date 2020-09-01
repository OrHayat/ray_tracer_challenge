//
// Created by משתמש1 on 01/09/2020.
//

#include <shared/env.hpp>
#include <shared/projectile.hpp>
#include <glm/glm.hpp>
#include <iostream>

int main(int argc, char** argv)
{
    glm::vec3 starting_pos(0.0f,1,0);
    glm::vec3 starting_speed=glm::normalize(glm::vec3(1,1,0));
    rtc::projectile p(starting_pos,starting_speed);
    glm::vec3 gravity=glm::vec3(0,-0.1,0);
    glm::vec3 wind=glm::vec3(-0.01,0,0);
    rtc::env env=rtc::env(gravity,wind);
    int tick=0;
    std::cout<<"tick "<<tick<<":["<<p.position.x<<","<<p.position.y<<","<<p.position.z<<"]"<<std::endl;
        while(p.position.y>0)
        {
            p=p.tick(env);
            tick++;
            std::cout<<"tick "<<tick<<":["<<p.position.x<<","<<p.position.y<<","<<p.position.z<<"]"<<std::endl;
        }
}