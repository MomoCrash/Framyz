// Created by Ethan G. on 22/06/2025.

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "framework.h"

class Transform {

    glm::mat4 m_transform;

    glm::vec3 m_forward;
    glm::vec3 m_right;
    glm::vec3 m_up;

    glm::vec3 m_scale;
    glm::vec3 m_position;

    glm::mat4 m_rotationMatrix;

    enum UpdateMask : uint8_t {
        POSITION,
        ROTATION,
        SCALE
    }; 

public:
    Transform();

    glm::mat4& getTransform();

    glm::vec3& getPosition();
    glm::vec3 const& forward();
    glm::vec3 backward();
    glm::vec3 const& right();
    glm::vec3 left();
    glm::vec3 const& up();
    glm::vec3 down();

    void setPosition(glm::vec3 const& position);
    void offsetPosition(glm::vec3 const& position);
    
    void rotateYPR(glm::vec3 const&rotation);
    
    void update();
    void reset();
    
};



#endif //TRANSFORM_H
