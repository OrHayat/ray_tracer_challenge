//
//

#ifndef RAY_TRACER_COLLISION_DATA_HPP
#define RAY_TRACER_COLLISION_DATA_HPP
#include <glm/glm.hpp>
#include <vector>
#include <optional>

struct shape;
struct collision_computation;
struct collision_data;
struct ray;

struct collision_data
{
    std::vector<float> t;
    shape& colided_shape;
    collision_data(shape& colided_shape);//t(std::vector<float>()){}//positions(std::vector<glm::vec3>()){}
    collision_data(std::vector<float>t, shape& colided_shape);
    collision_data(const collision_data& other);
    collision_data& operator= (const collision_data& other);
    std::optional<float>find_collision_value()const;
};

struct  collision_computation
{
    float t;
    shape& collided_shape;
    glm::vec3 intersection_point;
    glm::vec3 dir_from_intersection_to_eye;
    glm::vec3 intersection_point_normal;
    bool inside;
    collision_computation(float t,shape& colided_shape,glm::vec3 intersection_point,glm::vec3 dir_from_intersection_to_eye,glm::vec3 intersection_point_normal,bool inside);
    static collision_computation prepare_collision_computation(const ray &ray_from_eye,const float t,shape& collided_shape);
    //    static std::optional<struct collision_computation>   foo(const ray& r,const  collision_data& col_data);
    collision_computation()=delete;
};
#endif //RAY_TRACER_COLLISION_DATA_HPP
