// Created by Ethan G. on 22/06/2025.

#include "Transform.h"

Transform::Transform() {

    reset();
    
}

glm::mat4& Transform::getMatrix() {
    return m_matrix;
}

glm::vec3& Transform::getPosition()
{
    return m_position;
}

glm::vec3 & Transform::getRotation() {
    return m_rotation;
}

glm::vec3 const& Transform::forward()
{
    return m_forward;
}

glm::vec3 Transform::backward()
{
    return m_forward * -1.0f;
}

glm::vec3 const& Transform::right()
{
    return m_right;
}

glm::vec3 Transform::left()
{
    return m_right * -1.0f;
}

glm::vec3 const& Transform::up()
{
    return m_up;
}

glm::vec3 Transform::down()
{
    return m_up * -1.0f;
}

void Transform::setPosition(glm::vec3 const& position)
{
    m_position = position;

    m_isDirty = true;
    
}

void Transform::setPosition(float x, float y, float z)
{
    m_position.x = x;
    m_position.y = y;
    m_position.z = z;
    
    m_isDirty = true;
    
}

void Transform::offsetPosition(glm::vec3 const& position)
{
    m_position += position;

    m_isDirty = true;
    
}

void Transform::rotateYPR(glm::vec3 const& rotation)
{

    m_rotation += rotation;

    m_rotationMatrix = rotate(m_rotationMatrix, glm::radians(rotation.x), right());
    m_rotationMatrix = rotate(m_rotationMatrix, glm::radians(rotation.y), forward());
    m_rotationMatrix = rotate(m_rotationMatrix, glm::radians(rotation.z), up());

    glm::vec3 right     = { m_rotationMatrix[0][0], m_rotationMatrix[0][1], m_rotationMatrix[0][2] } ;
    glm::vec3 up        = { m_rotationMatrix[1][0], m_rotationMatrix[1][1], m_rotationMatrix[1][2] } ;
    glm::vec3 forward   = { m_rotationMatrix[2][0], m_rotationMatrix[2][1], m_rotationMatrix[2][2] } ;

    m_right     = glm::normalize(right  );
    m_up        = glm::normalize(up     );
    m_forward   = glm::normalize(forward);

    m_isDirty = true;
    
}

void Transform::update()
{

    if (!m_isDirty) return;
    
    m_matrix = glm::mat4(1.0f);
    
    m_matrix = scale(m_matrix, m_scale);
    m_matrix *= m_rotationMatrix;
    m_matrix = translate(m_matrix, m_position);
    
}

void Transform::reset()
{
    
    m_position  = glm::vec3(0.0f);
    m_rotation  = glm::vec3(0.0f);
    m_scale     = glm::vec3(1.0f);
    
    m_right     = glm::vec3( 1.0f, 0.0f, 0.0f );
    m_up        = glm::vec3( 0.0f, 1.0f, 0.0f );
    m_forward   = glm::vec3( 0.0f, 0.0f, 1.0f );

    m_rotationMatrix = glm::mat4(1.0f);

    m_isDirty = true;

}
