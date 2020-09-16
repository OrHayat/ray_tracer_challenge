//
//

#ifndef RAY_TRACER_COLLISION_DATA_HPP
#define RAY_TRACER_COLLISION_DATA_HPP
#include <glm/glm.hpp>
#include <vector>
#include <optional>

struct shape;
struct collision_data
{
    std::vector<float> t;
    const shape& colided_shape;
    collision_data(const shape& colided_shape);//t(std::vector<float>()){}//positions(std::vector<glm::vec3>()){}
    collision_data(std::vector<float>t,const shape& colided_shape);
    std::optional<float>find_collision_value();
};
#endif //RAY_TRACER_COLLISION_DATA_HPP
