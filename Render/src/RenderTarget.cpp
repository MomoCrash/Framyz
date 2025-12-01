#include "RenderTarget.h"

#include "RenderDevice.h"
#include "Mesh.h"
#include "RenderObject.h"
#include "RenderWindow.h"
#include "Texture.h"

RenderTarget::RenderTarget(RenderContext *context, RenderTargetInformation infos) {

    m_globalBuffer.proj = glm::mat4(1.0f);
    m_globalBuffer.view = glm::mat4(1.0f);

    size_t align = RenderDevice::getDynamicAlignment();
    size_t dBufferSize =  RenderPipeline::MAX_OBJECT_RENDERER * align;
    m_perObjectBuffer.model = (glm::mat4*)RenderDevice::alignedAlloc(dBufferSize, align);
    assert(m_perObjectBuffer.model);

    m_format = infos.format;
    m_imageType = static_cast<VkImageLayout>(infos.type);
    m_extent = VkExtent2D( infos.width, infos.height );
    m_offset = VkOffset2D( 0, 0 );
    m_informations = infos;
    
    m_clearValues[0].color = {{0.2f, 0.2f, 0.2f, 1.0f}};
    m_clearValues[1].depthStencil = {1.0f, 0};

    m_depthFormat = RenderDevice::getInstance()->findDepthFormat();
    m_sampleCount = RenderDevice::getInstance()->getMaxUsableSampleCount();
    m_renderContext = context;
    createRenderPass();
    
    createImages();
    
}

RenderTarget::~RenderTarget()
{
    VkDevice const & device = RenderDevice::getInstance()->getDevice();
    
    for (auto& image : m_images) 
        vkDestroyImage(device, image, nullptr);

    for (auto view : m_imageViews)
        vkDestroyImageView(device, view, nullptr);

    for (auto framebuffer : m_framebuffers)
        vkDestroyFramebuffer(device, framebuffer, nullptr);

    for (auto imagesMemory : m_imagesMemory)
        vkFreeMemory(device, imagesMemory, nullptr);
    
    vkDestroyRenderPass(device, m_renderPass, nullptr);
    
}

////////////////////////// RENDER OPERATIONS //////////////////////////////////////

void RenderTarget::beginDraw()
{
    
    VkCommandBuffer& buffer = m_renderContext->getCommandBuffer();
    
    // Starting a render pass
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = getRenderPass();
    renderPassInfo.framebuffer = m_framebuffers[getRenderContext().getCurrentFrame()];
    renderPassInfo.renderArea.offset = m_offset;
    renderPassInfo.renderArea.extent = m_extent;
    renderPassInfo.clearValueCount = static_cast<uint32_t>(m_clearValues.size());
    renderPassInfo.pClearValues = m_clearValues.data();

    vkCmdBeginRenderPass(buffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    
    VkViewport viewport{};
    viewport.x = m_offset.x;
    viewport.y = m_offset.y;
    viewport.width = m_extent.width;
    viewport.height = m_extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(buffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = m_offset;
    scissor.extent = m_extent;
    vkCmdSetScissor(buffer, 0, 1, &scissor);

    currentObject = 0;
    
}

void RenderTarget::drawObject(RenderPipeline& pipeline, RenderObject& object)
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
        0, 1, &pipeline.GetDescriptorSets(getRenderContext().getCurrentFrame()), 1, &dynamicOffset);
    vkCmdDrawIndexed(commandBuffer, object.getMesh()->getIndexCount(), 1, 0, 0, 0);

    currentObject++;

}

void RenderTarget::endDraw()
{
    VkCommandBuffer& commandBuffer = m_renderContext->getCommandBuffer();
    
    vkCmdEndRenderPass(commandBuffer);
    
}

////////////////////////// GETTER / SETTER //////////////////////////////////////

VkRenderPass& RenderTarget::getRenderPass()
{
    return m_renderPass;
}

RenderContext& RenderTarget::getRenderContext()
{
    return *m_renderContext;
}

VkImageView& RenderTarget::getImage(glm::uint32 frame)
{
    return m_imageViews[frame];
}

std::vector<VkImageView> const & RenderTarget::getImages() {
    return m_imageViews;
}

////////////////////////// CONTEXT CREATION //////////////////////////////////////

VkImageView RenderTarget::createImageView(VkImage image, VkFormat format, VkImageAspectFlagBits flags)
{
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = flags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;
    if (vkCreateImageView(RenderDevice::getInstance()->getDevice(), &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture image view!");
    }

    return imageView;
}

void RenderTarget::createRenderPass()
{
    
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format              = m_format;
    if (m_informations.useMSAA)
        colorAttachment.samples         = m_sampleCount;
    else
        colorAttachment.samples         = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp          = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp         = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp   = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp  = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout   = VK_IMAGE_LAYOUT_UNDEFINED;
    if (m_informations.useMSAA)
        colorAttachment.finalLayout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    else colorAttachment.finalLayout= m_imageType;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = RenderDevice::getInstance()->findDepthFormat();
    if (m_informations.useMSAA)
        depthAttachment.samples = m_sampleCount;
    else
        depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    
    VkAttachmentDescription colorAttachmentResolve{};
    VkAttachmentReference colorAttachmentResolveRef{};
    if (m_informations.useMSAA) {
        colorAttachmentResolve.format = m_informations.format;
        colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        colorAttachmentResolveRef.attachment = 2;
        colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    }
    
    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    if (m_informations.useMSAA)
        subpass.pResolveAttachments = &colorAttachmentResolveRef;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
    dependency.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    
    std::vector attachments = {colorAttachment, depthAttachment};
    if (m_informations.useMSAA)
        attachments.push_back(colorAttachmentResolve);
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(RenderDevice::getInstance()->getDevice(), &renderPassInfo, nullptr, &m_renderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }
    
}

void RenderTarget::createImages()
{
    
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = m_extent.width + m_offset.x;
    imageInfo.extent.height = m_extent.height + m_offset.x;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = m_format;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    m_images.resize(RenderWindow::MAX_FRAMES_IN_FLIGHT);
    m_imagesMemory.resize(RenderWindow::MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < m_images.size(); i++)
    {
        if (vkCreateImage(RenderDevice::getInstance()->getDevice(), &imageInfo, nullptr, &m_images[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image!");
        }

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(RenderDevice::getInstance()->getDevice(), m_images[i], &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = RenderDevice::getInstance()->findMemoryType(memRequirements.memoryTypeBits,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        if (vkAllocateMemory(RenderDevice::getInstance()->getDevice(), &allocInfo, nullptr, &m_imagesMemory[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate image memory!");
        }

        vkBindImageMemory(RenderDevice::getInstance()->getDevice(), m_images[i], m_imagesMemory[i], 0);
    }



    createImageViews();
    
}

void RenderTarget::createImageViews()
{
    
    m_imageViews.resize(RenderWindow::MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < m_imageViews.size(); i++) {

        m_imageViews[i] = createImageView(m_images[i], m_format, VK_IMAGE_ASPECT_COLOR_BIT);
    }

    createColorResources();
    createDepthResources();
    
    createFramebuffers();
}

void RenderTarget::createColorResources() {
    VkFormat colorFormat = m_informations.format;

    Texture::createImage(m_extent.width, m_extent.height,
        RenderDevice::getInstance()->getSampleCount(), colorFormat,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_colorImage, m_colorImageMemory);
    m_colorImageView = createImageView(m_colorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT);
}

void RenderTarget::createDepthResources() {
    Texture::createImage(m_extent.width, m_extent.height, RenderDevice::getInstance()->getSampleCount(),
            m_depthFormat, VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            m_depthImage, m_depthImageMemory);
    m_depthImageView = createImageView(m_depthImage, m_depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);

    Texture::transitionImageLayout(getRenderContext(), m_depthImage, m_depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
}

void RenderTarget::createFramebuffers()
{
    m_framebuffers.resize(RenderWindow::MAX_FRAMES_IN_FLIGHT);
    
    for (size_t i = 0; i < m_framebuffers.size(); i++) {

        std::array attachments = {
            m_colorImageView,
            m_depthImageView,
            m_imageViews[i],
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = getRenderPass();
        framebufferInfo.attachmentCount = attachments.size();
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = m_extent.width + m_offset.x;
        framebufferInfo.height = m_extent.height + m_offset.y;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(RenderDevice::getInstance()->getDevice(), &framebufferInfo, nullptr, &m_framebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}
