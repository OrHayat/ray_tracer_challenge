//
// Created by משתמש1 on 15/09/2020.
//

#include <iostream>
#include "collision_data.hpp"
#include "shape.hpp"
collision_data::collision_data( shape& colided_shape):t(std::vector<float>()),colided_shape(colided_shape){}
collision_data::collision_data(std::vector<float>t, shape& colided_shape):t(t),colided_shape(colided_shape){}
collision_data::collision_data(const collision_data& other):colided_shape(other.colided_shape),t(other.t){}

std::optional<float> collision_data::find_collision_value()const
{
    for(int i=0;i<this->t.size();i++)
    {
        if(this->t.at(i)>=0)
        {
            return std::make_optional(this->t.at(i));
        }
    }
    return std::optional<float>();
}
collision_data& collision_data::operator= (const  collision_data& other){
    if(this!=&other)
    {
        this->colided_shape=other.colided_shape;
        this->t=std::vector<float>(other.t);
    }
    return *this;
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

static std::optional<collision_computation> prepare_computations(const ray& r,const  collision_data& col_data)
{
    std::optional<float> col_t = col_data.find_collision_value();
    if(!col_t)
    {
        return {};
    }
    float t=col_t.value();
    shape& colided_shape=col_data.colided_shape;
    glm::vec3 intersection_point=r(t);
    glm::vec3 dir_from_intersection_to_eye=-r.dir;
    glm::vec3 intersection_point_normal=colided_shape.get_normal_at_point(intersection_point);
    bool inside=false;
    if(glm::dot(intersection_point_normal,dir_from_intersection_to_eye)<0)
    {
        intersection_point_normal=-intersection_point_normal;
        inside=true;
    }
    return std::make_optional(collision_computation(t,colided_shape,intersection_point,dir_from_intersection_to_eye,intersection_point_normal,inside));
}