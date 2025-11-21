// Created by Ethan G. on 22/06/2025.

#include "Transform.h"

Transform::Transform() {

    reset();
    
}

glm::mat4& Transform::getMatrix() {
    return m_matrix;
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

//========== Local Transform ==========//

void Transform::setLocalPosition(glm::vec3 const& position)
{
    m_localPosition = position;
    m_isDirty = true;
}

void Transform::setLocalPosition(float x, float y, float z)
{
    m_localPosition.x = x;
    m_localPosition.y = y;
    m_localPosition.z = z;
    
    m_isDirty = true;
}

void Transform::offsetLocalPosition(glm::vec3 const& position)
{
    m_localPosition += position;

    m_isDirty = true;
}

glm::vec3& Transform::getLocalPosition()
{
    return m_localPosition;
}

glm::vec3 & Transform::getLocalRotation()
{
    return m_localRotation;
}

glm::vec3 & Transform::getLocalScale() {
    return m_localScale;
}

void Transform::rotateLocalYPR(glm::vec3 const& rotation)
{

    m_localRotation += rotation;
    updateRotation();

    m_isDirty = true;
    
}

//========== World Transform ==========//

void Transform::setWorldPosition(glm::vec3 const &position)
{
    m_position = position;
    m_isDirty = true;
}

void Transform::setWorldPosition(float x, float y, float z) {
    m_position.x = x;
    m_position.y = y;
    m_position.z = z;
    
    m_isDirty = true;
}

void Transform::offsetWorldPosition(glm::vec3 const &position) {
    m_position += position;

    m_isDirty = true;
}

void Transform::rotateWorldYPR(glm::vec3 const &rotation) {
    
    m_rotation += rotation;
    updateRotation();

    m_isDirty = true;
    
}

glm::vec3 & Transform::getWorldPosition() {
    return m_position;
}

glm::vec3 & Transform::getWorldRotation() {
    return m_rotation;
}

glm::vec3 & Transform::getWorldScale() {
    return m_scale;
}

void Transform::setDirty() {
    m_isDirty = true;
}

void Transform::updateRotation() {
    
    m_localRotationMatrix = glm::mat4(1.0f);
    m_localRotationMatrix = glm::rotate(m_localRotationMatrix, glm::radians(m_localRotation.x), glm::vec3(1, 0, 0));
    m_localRotationMatrix = glm::rotate(m_localRotationMatrix, glm::radians(m_localRotation.y), glm::vec3(0, 1, 0));
    m_localRotationMatrix = glm::rotate(m_localRotationMatrix, glm::radians(m_localRotation.z), glm::vec3(0, 0, 1));

    m_worldRotationMatrix = glm::mat4(1.0f);
    m_worldRotationMatrix = glm::rotate(m_worldRotationMatrix, glm::radians(m_rotation.x), glm::vec3(1, 0, 0));
    m_worldRotationMatrix = glm::rotate(m_worldRotationMatrix, glm::radians(m_rotation.y), glm::vec3(0, 1, 0));
    m_worldRotationMatrix = glm::rotate(m_worldRotationMatrix, glm::radians(m_rotation.z), glm::vec3(0, 0, 1));

    m_rotationMatrix = m_worldRotationMatrix * m_localRotationMatrix;

    m_right   = glm::normalize(glm::vec3(m_rotationMatrix[0]));
    m_up      = glm::normalize(glm::vec3(m_rotationMatrix[1]));
    m_forward = glm::normalize(glm::vec3(m_rotationMatrix[2]));
    
}

void Transform::update()
{

    if (!m_isDirty) return;
    m_isDirty = false;

    // Model = T_world * R_world * T_local * R_local * Scale
    m_matrix = glm::mat4(1.0f);
    // m_matrix *= m_rotationMatrix;
    // m_matrix = translate(m_matrix, m_position);

    m_matrix = translate(m_matrix, m_localPosition);
    m_matrix *= m_localRotationMatrix;
    m_matrix = scale(m_matrix, m_localScale);

    m_matrix = translate(m_matrix, m_position);
    m_matrix *= m_worldRotationMatrix;
    m_matrix = scale(m_matrix, m_scale);
    
}

void Transform::reset()
{
    
    m_position          = glm::vec3(0.0f);
    m_localPosition     = glm::vec3(0.0f);
    m_rotation          = glm::vec3(0.0f);
    m_localRotation     = glm::vec3(0.0f);
    m_localScale        = glm::vec3(1.0f);
    m_scale             = glm::vec3(1.0f);
    
    m_right             = glm::vec3( 1.0f, 0.0f, 0.0f );
    m_up                = glm::vec3( 0.0f, 1.0f, 0.0f );
    m_forward           = glm::vec3( 0.0f, 0.0f, 1.0f );

    m_localRotationMatrix = glm::mat4(1.0f);
    m_worldRotationMatrix = glm::mat4(1.0f);
    m_rotationMatrix      = glm::mat4(1.0f);

    m_isDirty = true;

}
