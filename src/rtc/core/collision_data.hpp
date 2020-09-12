//
//

#ifndef RAY_TRACER_COLLISION_DATA_HPP
#define RAY_TRACER_COLLISION_DATA_HPP
struct collision_data
{
    std::vector<glm::vec3> positions;
//    int collision_count;
    const Shape & collided_shape;
    collision_data(std::vector<glm::vec3> positions,const Shape& collided_shape):
    positions(positions),collided_shape(collided_shape){}
};
#endif //RAY_TRACER_COLLISION_DATA_HPP
