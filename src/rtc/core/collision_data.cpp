//
// Created by משתמש1 on 15/09/2020.
//

#include <iostream>
#include "collision_data.hpp"
#include <rtc/shapes/shape.hpp>
collision_data::collision_data( shape& colided_shape):t(std::vector<float>()),colided_shape(&colided_shape){}
collision_data::collision_data(std::vector<float>t, shape& colided_shape):t(t),colided_shape(&colided_shape){}
collision_data::collision_data(const collision_data& other):colided_shape(other.colided_shape),t(other.t){}

std::optional<float> collision_data::find_collision_value()const
{
    for(int i=0;i<this->t.size();i++)
    {
        if(this->t.at(i)>=0.0001f)
        {
            return std::make_optional(this->t.at(i));
        }
    }
    return {};//std::optional<float>();
}
collision_data& collision_data::operator= (const  collision_data& other){
    if(this!=&other)
    {
        this->colided_shape=other.colided_shape;
        this->t=std::vector<float>(other.t);
    }
    return *this;
}

collision_data::collision_data() {

}

collision_computation::collision_computation(float t,
                                             shape& collided_shape,
                                             glm::vec3 intersection_point,
                                             glm::vec3 dir_from_intersection_to_eye,
                                             glm::vec3 intersection_point_normal,
                                             bool inside):t(t),
                                             collided_shape(collided_shape),
                                             intersection_point(intersection_point),
                                             dir_from_intersection_to_eye(dir_from_intersection_to_eye),
                                             intersection_point_normal(intersection_point_normal)
                                             ,inside(inside)
{
}

collision_computation collision_computation::prepare_collision_computation(const ray& ray_from_eye,const float t,collision_data& collision_result)
{
    glm::vec3 intersection_point=ray_from_eye(t);
    glm::vec3 dir_from_intersection_to_eye=-ray_from_eye.dir;
    glm::vec3 intersection_point_normal=glm::normalize(collision_result.colided_shape->get_normal_at_point(intersection_point,collision_result));
    bool inside=false;
    if(glm::dot(intersection_point_normal,dir_from_intersection_to_eye)<0)
    {
        intersection_point_normal=-intersection_point_normal;
        inside=true;
    }
    return collision_computation(t,*collision_result.colided_shape,intersection_point,dir_from_intersection_to_eye,intersection_point_normal,inside);
}
