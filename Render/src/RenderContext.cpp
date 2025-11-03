#include "RenderContext.h"

#include "RenderDevice.h"
#include "RenderWindow.h"


RenderContext::RenderContext(VkSurfaceKHR& surface)
{

    createCommandPool(surface);
    
    createCommandBuffer();

}

RenderContext::~RenderContext()
{
    
    vkDestroyCommandPool(RenderDevice::getInstance()->getDevice(), m_commandPool, nullptr);

}

void RenderContext::createCommandBuffer()
{

    m_commandBuffers.resize(RenderWindow::MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = m_commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) m_commandBuffers.size();

    if (vkAllocateCommandBuffers(RenderDevice::getInstance()->getDevice(), &allocInfo, m_commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
    
}

void RenderContext::createCommandPool(VkSurfaceKHR& surface)
{
    
    QueueFamilyIndices queueFamilyIndices = RenderDevice::getInstance()->
        findQueueFamilies(RenderDevice::getInstance()->getPhysicalDevice(), surface);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(RenderDevice::getInstance()->getDevice(), &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }
    
}

VkCommandBuffer RenderContext::beginSingleTimeCommands()
{

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = m_commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(RenderDevice::getInstance()->getDevice(), &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void RenderContext::endSingleTimeCommands(VkCommandBuffer commandBuffer)
{
    
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(RenderDevice::getInstance()->getGraphicQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(RenderDevice::getInstance()->getGraphicQueue());

    vkFreeCommandBuffers(RenderDevice::getInstance()->getDevice(), m_commandPool, 1, &commandBuffer);
    
}

void RenderContext::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{

    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = 0; // Optional
    copyRegion.dstOffset = 0; // Optional
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    endSingleTimeCommands(commandBuffer);
    
}

void RenderContext::setCurrentFrame(uint32_t* frame)
{
    m_currentFrame = frame;
}

uint32_t RenderContext::getCurrentFrame()
{
    return *m_currentFrame;
}

VkCommandBuffer& RenderContext::getCommandBuffer()
{
    return m_commandBuffers[*m_currentFrame];
}

