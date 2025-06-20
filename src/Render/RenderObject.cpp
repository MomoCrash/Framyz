#include "RenderObject.h"

RenderObject::RenderObject(Mesh* mesh)
    : m_transform(glm::mat4(1.0f))
{
    m_mesh = mesh;

    reset();
}

RenderObject::~RenderObject()
{
}

Mesh const* RenderObject::getMesh()
{
    return m_mesh;
}

glm::mat4 const& RenderObject::getTransform()
{
    return m_transform;
}

glm::vec3& RenderObject::getPosition()
{
    return m_position;
}

glm::vec3 const& RenderObject::forward()
{
    return m_forward;
}

glm::vec3 RenderObject::backward()
{
    return m_forward * -1.0f;
}

glm::vec3 const& RenderObject::right()
{
    return m_right;
}

glm::vec3 RenderObject::left()
{
    return m_right * -1.0f;
}

glm::vec3 const& RenderObject::up()
{
    return m_up;
}

glm::vec3 RenderObject::down()
{
    return m_up * -1.0f;
}

void RenderObject::setPosition(glm::vec3 const& position)
{
    m_position = position;
}

void RenderObject::offsetPosition(glm::vec3 const& position)
{
    m_position += position;
}

void RenderObject::rotateYPR(glm::vec3 const& rotation)
{

    glm::vec3 RIGHT		= { 1, 0, 0 };
    glm::vec3 UP			= { 0, 1, 0 };
    glm::vec3 FORWARD	= { 0, 0, 1 };

    m_rotationMatrix = rotate(m_rotationMatrix, rotation.x, right());
    m_rotationMatrix = rotate(m_rotationMatrix, rotation.y, forward());
    m_rotationMatrix = rotate(m_rotationMatrix, rotation.z, up());

    m_right     = { m_rotationMatrix[0][0], m_rotationMatrix[0][1], m_rotationMatrix[0][2] };
    m_up        = { m_rotationMatrix[1][0], m_rotationMatrix[1][1], m_rotationMatrix[1][2] };
    m_forward   = { m_rotationMatrix[2][0], m_rotationMatrix[2][1], m_rotationMatrix[2][2] };
    
}

void RenderObject::update()
{
    
    m_transform = glm::mat4(1.0f);
    
    m_transform = scale(m_transform, m_scale);
    m_transform *= m_rotationMatrix;
    m_transform = translate(m_transform, m_position);
    
}

void RenderObject::reset()
{
    
    m_position  = glm::vec3(0.0f);
    m_scale     = glm::vec3(1.0f);
    
    m_right     = glm::vec3( 1.0f, 0.0f, 0.0f );
    m_up        = glm::vec3( 0.0f, 1.0f, 0.0f );
    m_forward   = glm::vec3( 0.0f, 0.0f, 1.0f );

    m_rotationMatrix = glm::mat4(1.0f);

}
