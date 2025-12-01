#pragma once
#include <string>
#include "framework.h"

class RenderContext;
class RenderTarget;
class RenderWindow;

class Texture
{
public:
    Texture(RenderTarget& renderTarget, std::string const& textureFile);
    ~Texture();

    VkImageView& getImageView();
    
    static const inline std::string TEXTURE_FOLDER = "..\\..\\res\\textures\\";
    static void createImage(uint32_t width, uint32_t height, VkSampleCountFlagBits numSample, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    static void transitionImageLayout(RenderContext& renderWindow, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

private:
    VkImage m_textureImage;
    VkImageView m_textureImageView;
    
    VkDeviceMemory m_textureImageMemory;

    void createTextureImageView(RenderTarget& renderWindow);

    void copyBufferToImage(RenderContext& renderWindow, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
};
