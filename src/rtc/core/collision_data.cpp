//
// Created by משתמש1 on 15/09/2020.
//

#include "collision_data.hpp"
#include "shape.hpp"
collision_data::collision_data(const shape& colided_shape):t(std::vector<float>()),colided_shape(colided_shape){}
collision_data::collision_data(std::vector<float>t,const shape& colided_shape):t(t),colided_shape(colided_shape){}

std::optional<float> collision_data::find_collision_value(){
    for(int i=0;i<this->t.size();i++)
    {
        if(this->t.at(i)>=0)
        {
            return std::optional<float>(this->t.at(i));
        }
    }
    return std::optional<float>();
}