#include "Mesh.h"

#include "RenderDevice.h"
#include "RenderWindow.h"

Vertex::Vertex(): position(0, 0, 0), normal(0, 0, 0), texCoords(0, 0) {}

Vertex::Vertex(float x, float y, float z, float xN, float yN, float zN, float xT, float yT)
    : position(x, y, z), normal(xN, yN, zN), texCoords(xT, yT) {}

Mesh::Mesh(const char* assetName, RenderContext& context, MeshData* dMesh) : m_vertexBuffer(nullptr)
{

    m_assetName = assetName;
    m_meshData = dMesh;

    uint64_t vSize = sizeof(dMesh->Vertices[0]) * dMesh->Vertices.size();
    assert(vSize > 0);

    VkBuffer stagingBuffer = nullptr;
    VkDeviceMemory stagingBufferMemory = nullptr;
    RenderDevice::getInstance()->createBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer, stagingBufferMemory, vSize);

    void* data;
    vkMapMemory(RenderDevice::getInstance()->getDevice(), stagingBufferMemory, 0, vSize, 0, &data);
    memcpy(data, dMesh->Vertices.data(), vSize);
    vkUnmapMemory(RenderDevice::getInstance()->getDevice(), stagingBufferMemory);

    RenderDevice::getInstance()->createBuffer(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        m_vertexBuffer, m_vertexUploader, vSize);

    context.copyBuffer(stagingBuffer, m_vertexBuffer, vSize);

    VkDeviceSize iSize = sizeof(dMesh->Indices[0]) * dMesh->Indices.size();
    
    vkMapMemory(RenderDevice::getInstance()->getDevice(), stagingBufferMemory, 0, iSize, 0, &data);
    memcpy(data, dMesh->Indices.data(), iSize);
    vkUnmapMemory(RenderDevice::getInstance()->getDevice(), stagingBufferMemory);

    RenderDevice::getInstance()->createBuffer(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        m_indexBuffer, m_indexBufferUploader, iSize);

    context.copyBuffer(stagingBuffer, m_indexBuffer, iSize);

    vkDestroyBuffer(RenderDevice::getInstance()->getDevice(), stagingBuffer, nullptr);
    vkFreeMemory(RenderDevice::getInstance()->getDevice(), stagingBufferMemory, nullptr);
    
}

Mesh::~Mesh()
{

    vkDestroyBuffer(RenderDevice::getInstance()->getDevice(), m_indexBuffer, nullptr);
    vkFreeMemory(RenderDevice::getInstance()->getDevice(), m_indexBufferUploader, nullptr);
    
    vkDestroyBuffer(RenderDevice::getInstance()->getDevice(), m_vertexBuffer, nullptr);
    vkFreeMemory(RenderDevice::getInstance()->getDevice(), m_vertexUploader, nullptr);
    
}

VkBuffer const& Mesh::getVertexBuffer() const
{
    return m_vertexBuffer;
}

VkBuffer const& Mesh::getIndexBuffer() const
{
    return m_indexBuffer;
}

std::vector<Vertex> const& Mesh::getVertices() const
{
    return m_meshData->Vertices;
}

glm::uint32 Mesh::getIndexCount() const
{
    return m_meshData->Indices.size();
}

const char * Mesh::getAssetName() const {
    return m_assetName;
}
