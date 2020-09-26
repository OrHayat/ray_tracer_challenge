//
// Created by orhayat on 26/09/2020.
//

#ifndef RAY_TRACER_MATERIAL_H
#define RAY_TRACER_MATERIAL_H


struct material {
    glm::vec4 ka=glm::vec4(0);
    glm::vec4 ks=glm::vec4(0);;
    glm::vec4 kd=glm::vec4(0);;
    glm::vec4 kr=glm::vec4(0);;
    glm::vec4 kt=glm::vec4(0);;
    float shininess=0.0f;
};


#endif //RAY_TRACER_MATERIAL_H
