#pragma once

#include "framework.h"
#include "RenderContext.h"
#include "RenderPipeline.h"

class RenderObject;
class RenderPipeline;
class RenderWindow;

class RenderTarget
{
public:
    
    RenderTarget(VkFormat format, int width, int height);
    ~RenderTarget();

    void beginDraw();
    void drawObject(RenderPipeline& pipeline, RenderObject& object);
    void endDraw();
    
    VkRenderPass& getRenderPass();
    void setRenderContext(RenderContext* renderContext);
    RenderContext& getRenderContext();
    VkImageView& getImage(glm::uint32 frame);

    VkImageView createImageView(VkImage image, VkFormat format);

private:

    VkClearValue m_clearColor = { {{0.1f, 0.1f, 0.1f, 1.0f}} };
    UniformBufferObject m_globalBuffer;
    UboDataDynamic m_perObjectBuffer;
    int currentObject = 0;

    RenderContext* m_renderContext;  

    VkFormat m_format;
    VkExtent2D m_extent{};
    VkOffset2D m_offset{};
    
    VkRenderPass m_renderPass;
    
    std::vector<VkImage>        m_images;
    std::vector<VkImageView>    m_imageViews;
    std::vector<VkFramebuffer>  m_framebuffers;
    std::vector<VkDeviceMemory> m_imagesMemory;

    void createRenderPass();

    void createImages();
    void createImageViews();
    void createFramebuffers();

};
