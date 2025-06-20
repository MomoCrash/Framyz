// Created by Ethan G. on 20/06/2025.

#ifndef FRAMEWORK_RENDER_H
#define FRAMEWORK_RENDER_H

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vulkan/vulkan.h>

#include <im_gui/imgui.h>
#include <im_gui/imgui_internal.h>
#include <im_gui/backends/imgui_impl_glfw.h>
#include <im_gui/backends/imgui_impl_vulkan.h>

#include "im_nodeflow/include/ImNodeFlow.h"

#include <iostream>

#endif //FRAMEWORK_RENDER_H
