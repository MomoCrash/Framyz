#include "RenderWindow.h"

#include <algorithm>
#include <chrono>

#include "Inputs.h"
#include "RenderDevice.h"
#include "Mesh.h"
#include "RenderObject.h"
#include "RenderPipeline.h"
#include "Sampler.h"
#include "Texture.h"

RenderWindow::RenderWindow(const char* name, const int width, const int height)
    : Window(name, width, height), m_device(&RenderDevice::getInstance()->getDevice())
{

    m_globalBuffer.proj = glm::mat4(1.0f);
    m_globalBuffer.view = glm::mat4(1.0f);

    size_t align = RenderDevice::getDynamicAlignment();
    size_t dBufferSize = 125 * align;
    m_perObjectBuffer.model = (glm::mat4*)RenderDevice::alignedAlloc(dBufferSize, align);
    assert(m_perObjectBuffer.model);

    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
    {
        auto app = reinterpret_cast<RenderWindow*>(glfwGetWindowUserPointer(window));
        app->framebufferResized = true;
    });

    m_clearValues[0].color = {{0.2f, 0.2f, 0.2f, 1.0f}};
    m_clearValues[1].depthStencil = {1.0f, 0};

    createSurface();

    RenderDevice::getInstance()->setupPhysicalDevice(getSurface());
    RenderDevice::getInstance()->setupLogicalDevice(getSurface());

    m_renderContext = new RenderContext(getSurface());
    m_renderContext->setCurrentFrame(&currentFrame);

    m_depthFormat = RenderDevice::getInstance()->findDepthFormat();

    createSwapChain();

    RenderTargetInformation creationInfos{};
    creationInfos.format = m_swapChainImageFormat;
    creationInfos.width = width;
    creationInfos.height = height;
    creationInfos.useMSAA = true;
    m_renderTarget = new RenderTarget(m_renderContext, creationInfos);

    createColorResources();
    createDepthResources();
    
    createFramebuffers();
    
    createSyncObjects();
    
}

RenderWindow::~RenderWindow()
{

    delete m_renderContext;
    delete m_renderTarget;

    cleanupSwapChain();

    vkDestroyImageView(RenderDevice::getInstance()->getDevice(), m_depthImageView, nullptr);
    vkDestroyImage(RenderDevice::getInstance()->getDevice(), m_depthImage, nullptr);
    vkFreeMemory(RenderDevice::getInstance()->getDevice(), m_depthImageMemory, nullptr);
    
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        
        vkDestroySemaphore(*m_device, m_renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(*m_device, m_imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(*m_device, m_inFlightFences[i], nullptr);
        
    }

    vkDestroySurfaceKHR(RenderDevice::getInstance()->getVulkanInstance(), m_surface, nullptr);
}

void RenderWindow::createSurface()
{
    m_surface = *RenderDevice::getInstance()->createSurface(*this);
}

void RenderWindow::createSwapChain()
{
    SwapChainSupportDetails swapChainSupport = RenderDevice::getInstance()->querySwapChainSupport(RenderDevice::getInstance()->getPhysicalDevice(), m_surface);

    VkSurfaceFormatKHR surfaceFormat    = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode        = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent                   = chooseSwapExtent(swapChainSupport.capabilities);

    m_swapChainImageFormat = surfaceFormat.format;
    m_swapChainExtent = extent;

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = m_surface;

    createInfo.minImageCount    = imageCount;
    createInfo.imageFormat      = surfaceFormat.format;
    createInfo.imageColorSpace  = surfaceFormat.colorSpace;
    createInfo.imageExtent      = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = RenderDevice::getInstance()->findQueueFamilies(RenderDevice::getInstance()->getPhysicalDevice(), m_surface);
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;
    
    VkResult result = vkCreateSwapchainKHR(*m_device, &createInfo, nullptr, &m_swapchain);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }
    
    vkGetSwapchainImagesKHR(*m_device, m_swapchain, &imageCount, nullptr);
    m_swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(*m_device, m_swapchain, &imageCount, m_swapChainImages.data());

    createImageViews();

}

void RenderWindow::createImageViews()
{
    m_swapChainImageViews.resize(m_swapChainImages.size());

    for (size_t i = 0; i < m_swapChainImages.size(); i++) {

        m_swapChainImageViews[i] = getRenderTarget().createImageView(m_swapChainImages[i], m_swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
    }
}

void RenderWindow::createColorResources() {
		VkFormat colorFormat = m_swapChainImageFormat;

		Texture::createImage(m_swapChainExtent.width, m_swapChainExtent.height,
		    RenderDevice::getInstance()->getSampleCount(), colorFormat,
		    VK_IMAGE_TILING_OPTIMAL,
		    VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_colorImage, m_colorImageMemory);
		m_colorImageView = m_renderTarget->createImageView(m_colorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT);
	}

void RenderWindow::createDepthResources() {
    Texture::createImage(m_swapChainExtent.width, m_swapChainExtent.height,
        RenderDevice::getInstance()->getSampleCount(), m_depthFormat,
        VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            m_depthImage, m_depthImageMemory);
    m_depthImageView = m_renderTarget->createImageView(m_depthImage, m_depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);

    Texture::transitionImageLayout(getRenderContext(), m_depthImage, m_depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
}

void RenderWindow::createFramebuffers()
{
    m_swapChainFramebuffers.resize(m_swapChainImageViews.size());

    for (size_t i = 0; i < m_swapChainImageViews.size(); i++) {
        std::array attachments = {
            m_colorImageView,
            m_depthImageView,
            m_swapChainImageViews[i],
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = m_renderTarget->getRenderPass();
        framebufferInfo.attachmentCount = attachments.size();
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = m_swapChainExtent.width;
        framebufferInfo.height = m_swapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(*m_device, &framebufferInfo, nullptr, &m_swapChainFramebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}

void RenderWindow::createSyncObjects()
{
    
    m_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(*m_device, &semaphoreInfo, nullptr, &m_imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(*m_device, &semaphoreInfo, nullptr, &m_renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(*m_device, &fenceInfo, nullptr, &m_inFlightFences[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create synchronization objects for a frame!");
            }
    }
    
}

void RenderWindow::recreateSwapchain()
{
    // TODO Remove that because pause app when minimized
    int width = 0, height = 0;
    glfwGetFramebufferSize(m_window, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(m_window, &width, &height);
        glfwWaitEvents();
    }
    
    vkDeviceWaitIdle(*m_device);

    cleanupSwapChain();

    createSwapChain();
    createImageViews();
    createColorResources();
    createDepthResources();
    createFramebuffers();
}

VkSurfaceFormatKHR RenderWindow::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR RenderWindow::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes) {
        // We ue mailing method because remove tearing and render fastest as possible
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D RenderWindow::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(m_window, &width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

void RenderWindow::cleanupSwapChain()
{

    vkDestroyImageView(*m_device, m_depthImageView, nullptr);
    vkDestroyImage(*m_device, m_depthImage, nullptr);
    vkFreeMemory(*m_device, m_depthImageMemory, nullptr);

    vkDestroyImageView(*m_device, m_colorImageView, nullptr);
    vkDestroyImage(*m_device, m_colorImage, nullptr);
    vkFreeMemory(*m_device, m_colorImageMemory, nullptr);
    
    for (size_t i = 0; i < m_swapChainFramebuffers.size(); i++) {
        vkDestroyFramebuffer(*m_device, m_swapChainFramebuffers[i], nullptr);
    }

    for (size_t i = 0; i < m_swapChainImageViews.size(); i++) {
        vkDestroyImageView(*m_device, m_swapChainImageViews[i], nullptr);
    }

    vkDestroySwapchainKHR(*m_device, m_swapchain, nullptr);
}

uint32_t RenderWindow::flushCommand()
{
    /*
     * Wait for the previous frame to finish
     * Acquire an image from the swap chain
     * Record a command buffer which draws the scene onto that image
     * Submit the recorded command buffer
     * Present the swap chain image
    */

    // Refer to https://vulkan-tutorial.com/en/Drawing_a_triangle/Drawing/Rendering_and_presentation
    // Semaphores for sync the GPU with signal
    // And Fences for sync the CPU with GPU
    
    vkWaitForFences(*m_device, 1, &m_inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(*m_device, m_swapchain, UINT64_MAX, m_imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        framebufferResized = false;
        recreateSwapchain();
        return imageIndex;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    vkResetFences(*m_device, 1, &m_inFlightFences[currentFrame]);
    
    vkResetCommandBuffer(m_renderContext->getCommandBuffer(), 0);
    return imageIndex;
}

void RenderWindow::update(CameraInformation const& cam)
{

    // Update Uniform Bffer
    auto now = std::chrono::high_resolution_clock::now();
    
    frameCounter++;

    // m_globalBuffer.view = glm::lookAt(glm::vec3(0.0f, 0.0f,  -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // m_globalBuffer.proj = glm::perspective(glm::radians(70.0f), width / height, 0.1f, 256.0f);

    m_globalBuffer.proj = glm::perspective(glm::radians(cam.Fov), cam.AspectRatio, cam.ZNear, cam.ZFar);
    m_globalBuffer.view = cam.View;
    m_globalBuffer.proj[1][1] *= -1.0f;
    
    float fpsTimer = (float)(std::chrono::duration<double, std::milli>(now - lastTime).count());

    if (fpsTimer > 1000.0f)
    {
        
        uint32_t fps = static_cast<uint32_t>((float)frameCounter * (1000.0f / fpsTimer));
        
        std::string name = "FPS : " + std::to_string(fps);
        glfwSetWindowTitle(m_window, name.c_str());
        
        frameCounter = 0;
        lastTime = now;
        
    }

}

void RenderWindow::beginFrame()
{

    m_imageIndex = flushCommand();
    VkCommandBuffer& buffer = m_renderContext->getCommandBuffer();
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT; // Optional
    beginInfo.pInheritanceInfo = nullptr; // Optional

    // Flags can be
    // VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT: This is a secondary command buffer that will be entirely within a single render pass.
    // VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT: The command buffer will be rerecorded right after executing it once.
    
    if (vkBeginCommandBuffer(buffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }
    
}

void RenderWindow::beginDraw()
{
    VkCommandBuffer& buffer = m_renderContext->getCommandBuffer();
    
    // Starting a render pass
    /* TODO | CEST VRAIMENT ETRANGE CAR
     * TODO | NORMALEMENT CA DEVRAIT ETRE LE CALL QUI EST DANS RENDER TARGET */
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_renderTarget->getRenderPass();
    renderPassInfo.framebuffer = m_swapChainFramebuffers[m_imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = m_swapChainExtent;
    renderPassInfo.clearValueCount = static_cast<uint32_t>(m_clearValues.size());
    renderPassInfo.pClearValues = m_clearValues.data();

    vkCmdBeginRenderPass(buffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(m_swapChainExtent.width);
    viewport.height = static_cast<float>(m_swapChainExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(buffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = m_swapChainExtent;
    vkCmdSetScissor(buffer, 0, 1, &scissor);

    currentObject = 0;
}

void RenderWindow::drawObject(RenderPipeline& pipeline, RenderObject& object)
{
    glm::mat4* modelMat = (glm::mat4*)((uint64_t)m_perObjectBuffer.model + (currentObject * RenderDevice::getDynamicAlignment()));
    *modelMat = object.getTransform();

    pipeline.UpdatePerObject(&m_perObjectBuffer, getRenderContext().getCurrentFrame());
    
    VkCommandBuffer& commandBuffer = m_renderContext->getCommandBuffer();
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getGraphicsPipeline());

    VkBuffer vertexBuffers[] = {
        object.getMesh()->getVertexBuffer()
    };
    
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(commandBuffer, object.getMesh()->getIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);

    uint32_t dynamicOffset = currentObject * static_cast<uint32_t>(RenderDevice::getDynamicAlignment());
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getGraphicsPipelineLayout(),
        0, 1, &pipeline.GetDescriptorSets(currentFrame), 1, &dynamicOffset);
    vkCmdDrawIndexed(commandBuffer, object.getMesh()->getIndexCount(), 1, 0, 0, 0);

    currentObject++;

}

void RenderWindow::display()
{
    VkCommandBuffer& buffer = m_renderContext->getCommandBuffer();

    if (vkEndCommandBuffer(buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
    
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {m_imageAvailableSemaphores[currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_renderContext->getCommandBuffer();

    VkSemaphore signalSemaphores[] = {m_renderFinishedSemaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(RenderDevice::getInstance()->getGraphicQueue(), 1, &submitInfo, m_inFlightFences[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }
    
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {m_swapchain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &m_imageIndex;
    presentInfo.pResults = nullptr; // Optional

    VkResult result = vkQueuePresentKHR(RenderDevice::getInstance()->getGraphicQueue(), &presentInfo);
    
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
        framebufferResized = false;
        recreateSwapchain();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }
    
    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

    Input::Update(m_window);
    glfwPollEvents();
    
}

void RenderWindow::endDraw()
{
    VkCommandBuffer& buffer = m_renderContext->getCommandBuffer();
    vkCmdEndRenderPass(buffer);
}


RenderContext& RenderWindow::getRenderContext()
{
    return *m_renderContext;
}

UniformBufferObject& RenderWindow::GetGlobalBuffer()
{
    return m_globalBuffer;
}

uint32_t RenderWindow::getCurrentFrame()
{
    return currentFrame;
}

const VkExtent2D& RenderWindow::getExtent2D()
{
    return m_swapChainExtent;
}

VkSurfaceKHR& RenderWindow::getSurface()
{
    return m_surface;
}

RenderTarget& RenderWindow::getRenderTarget()
{
    return *m_renderTarget;
}

bool RenderWindow::hasStencilComponent(VkFormat format)
{
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

bool RenderWindow::shouldClose()
{
    return glfwWindowShouldClose(m_window);
}
