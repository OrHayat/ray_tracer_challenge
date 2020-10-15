//
// Created by משתמש1 on 15/09/2020.
//

#include <iostream>
#include "collision_data.hpp"
#include <rtc/shapes/shape.hpp>
#include "rtc/Scene.hpp"
#include <unordered_map>
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
                                             bool inside,
                                             glm::vec3 reflected_ray,
                                             float n1,float n2):t(t),
                                             collided_shape(collided_shape),
                                             intersection_point(intersection_point),
                                             dir_from_intersection_to_eye(dir_from_intersection_to_eye),
                                             intersection_point_normal(intersection_point_normal),
                                             inside(inside),
                                             reflected_ray(reflected_ray),
                                             n1(n1),n2(n2)
{
}

collision_computation collision_computation::prepare_collision_computation(const ray& ray_from_eye,
                                                                           const float t,
                                                                           collision_data& collision_result,
                                                                           int collided_mesh_index,
                                                                           collision_with_scene_result& collision_with_scene,
                                                                           const std::vector<shape*>& objects
                                                                           )
{
    float n1=1.0f,n2=1.0f;
    std::unordered_map<int,int>intersection_count;
    for(int i=0;i<collision_with_scene.data.size();i++)
    {
        if(i==collided_mesh_index) {
            for (int j = i - 1; j >= 0; j--) {
                int cur_count = intersection_count[collision_with_scene.data.at(j).collided_shape->id];
                if (cur_count % 2 == 1) {
                    n1 = collision_with_scene.data.at(j).collided_shape->mat.refractive_index;
                    break;
                }
            }
        }
        intersection_count[collision_with_scene.data.at(i).collided_shape->id]++;
        if(i==collided_mesh_index) {
            for (int j = i - 1; j >= 0; j--) {
                int cur_count = intersection_count[collision_with_scene.data.at(j).collided_shape->id];
                if (cur_count % 2 == 1) {
                    n2 = collision_with_scene.data.at(j).collided_shape->mat.refractive_index;
                    break;
                }
            }
        }

    }
    glm::vec3 intersection_point=ray_from_eye(t);
    glm::vec3 dir_from_intersection_to_eye=-ray_from_eye.dir;
    glm::vec3 intersection_point_normal=glm::normalize(collision_result.colided_shape->get_normal_at_point(intersection_point,collision_result));
    bool inside=false;
    if(glm::dot(intersection_point_normal,dir_from_intersection_to_eye)<0)
    {
        intersection_point_normal=-intersection_point_normal;
        inside=true;
    }
    glm::vec3 reflected_ray=glm::reflect(glm::normalize(ray_from_eye.dir),intersection_point_normal);
    return collision_computation(t,*collision_result.colided_shape,intersection_point,dir_from_intersection_to_eye,intersection_point_normal,inside,reflected_ray,n1,n2);
}
