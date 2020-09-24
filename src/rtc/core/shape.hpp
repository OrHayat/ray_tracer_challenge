//
// Created by USER on 14/09/2020.
//

#ifndef RAY_TRACER_SHAPE_HPP
#define RAY_TRACER_SHAPE_HPP
#include <rtc/core/collision_data.hpp>
#include <rtc/core/ray.hpp>
#include <glm/glm.hpp>
struct shape{
    glm::mat4 model=glm::mat4();
    glm::mat4 model_inv=glm::mat4();
    glm::vec4 ka=glm::vec4(0);
    glm::vec4 ks=glm::vec4(0);;
    glm::vec4 kd=glm::vec4(0);;
    glm::vec4 kr=glm::vec4(0);;
    glm::vec4 kt=glm::vec4(0);;
    float shininess=0.0f;
    virtual collision_data collide(ray r)=0;
    virtual glm::vec3 get_normal_at_point(glm::vec3 point)=0;
    void set_model(const glm::mat4& model)
    {
        this->model=model;
        this->model_inv=glm::inverse(model);
    }
};
#endif //RAY_TRACER_SHAPE_HPP
