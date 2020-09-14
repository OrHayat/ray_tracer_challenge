//
// Created by משתמש1 on 15/09/2020.
//

#include "collision_data.hpp"
#include "shape.hpp"
collision_data::collision_data(const shape& colided_shape):t(std::vector<float>()),colided_shape(colided_shape){}//positions(std::vector<glm::vec3>()){}
collision_data::collision_data(std::vector<float>t,const shape& colided_shape):t(t),colided_shape(colided_shape){}
