//
// Created by משתמש1 on 15/09/2020.
//

#include <iostream>
#include "collision_data.hpp"
#include "shape.hpp"
collision_data::collision_data( shape& colided_shape):t(std::vector<float>()),colided_shape(colided_shape){}
collision_data::collision_data(std::vector<float>t, shape& colided_shape):t(t),colided_shape(colided_shape){}
collision_data::collision_data(const collision_data& other):colided_shape(other.colided_shape),t(other.t){}

std::optional<float> collision_data::find_collision_value(){
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