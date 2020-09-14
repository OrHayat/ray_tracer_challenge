//
//

#ifndef RAY_TRACER_COLLISION_DATA_HPP
#define RAY_TRACER_COLLISION_DATA_HPP
#include <glm/glm.hpp>
#include <vector>

struct collision_data
{
    std::vector<glm::vec3> positions;
    collision_data():positions(std::vector<glm::vec3>()){}
    collision_data(std::vector<glm::vec3>positions):positions(positions){}
};
#endif //RAY_TRACER_COLLISION_DATA_HPP
