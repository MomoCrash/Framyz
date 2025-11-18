// Created by Ethan G. on 22/06/2025.

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "framework.h"

class Transform {

    glm::mat4 m_matrix;
    
    glm::vec3 m_forward;
    glm::vec3 m_right;
    glm::vec3 m_up;

    glm::vec3 m_scale;
    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::mat4 m_worldRotationMatrix;

    glm::vec3 m_localScale;
    glm::vec3 m_localPosition;
    glm::vec3 m_localRotation;
    glm::mat4 m_localRotationMatrix;
    
    glm::mat4 m_rotationMatrix;

    bool      m_isDirty = false;

    enum UpdateMask : uint8_t {
        POSITION,
        ROTATION,
        SCALE
    }; 

public:
    Transform();

    //==== Final Transformation
    glm::mat4&  getMatrix();

    glm::vec3   const& forward();
    glm::vec3   backward();
    
    glm::vec3   const& right();
    glm::vec3   left();
    
    glm::vec3   const& up();
    glm::vec3   down();

    //==== Local Transform
    void        setLocalPosition(glm::vec3 const& position);
    void        setLocalPosition(float x, float y, float z);
    void        offsetLocalPosition(glm::vec3 const& position);
    
    void        rotateLocalYPR(glm::vec3 const&rotation);
    glm::vec3&  getLocalPosition();
    glm::vec3&  getLocalRotation();

    //==== World Transform
    void        setWorldPosition(glm::vec3 const& position);
    void        setWorldPosition(float x, float y, float z);
    void        offsetWorldPosition(glm::vec3 const& position);
    
    void        rotateWorldYPR(glm::vec3 const&rotation);
    glm::vec3&  getWorldPosition();
    glm::vec3&  getWorldRotation();

    
    void        setDirty();
    void        updateRotation();
    void        update();
    void        reset();
    
};



#endif //TRANSFORM_H
