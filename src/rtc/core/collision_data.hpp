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
    shape& colided_shape;
    collision_data(shape& colided_shape);//t(std::vector<float>()){}//positions(std::vector<glm::vec3>()){}
    collision_data(std::vector<float>t, shape& colided_shape);
    collision_data(const collision_data& other);
    collision_data& operator= (const collision_data& other);
    std::optional<float>find_collision_value();
};
#endif //RAY_TRACER_COLLISION_DATA_HPP
