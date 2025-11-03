# Dear ImGui
set(IMGUI_DIR /im_gui)
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends)

set(IM_GUI_LIB
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
)

add_compile_definitions(IMGUI_DEFINE_MATH_OPERATORS)

#ImNode Flow
set(IMNODEFLOW_DIR ${PROJECT_SOURCE_DIR}/External/im_nodeflow)

# SET SOURCES FILES FOR PROJECT COMPILABLE LIBS
file(GLOB_RECURSE BUILTIN_EXTERNALS ${PROJECT_SOURCE_DIR}/External/*.cpp
        ${PROJECT_SOURCE_DIR}/External/*.h
        ${PROJECT_SOURCE_DIR}/External/*.inl)

###### The configurations we support
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Distribution")

# Number of bits to use in ObjectLayer. Can be 16 or 32.
set(OBJECT_LAYER_BITS 16)

# Select X86 processor features to use, by default the library compiles with AVX2, if everything is off it will be SSE2 compatible.
set(USE_SSE4_1 ON)
set(USE_SSE4_2 ON)
set(USE_AVX ON)
set(USE_AVX2 ON)
set(USE_AVX512 OFF)
set(USE_LZCNT ON)
set(USE_TZCNT ON)
set(USE_F16C ON)
set(USE_FMADD ON)

include(FetchContent)
# Include Jolt
FetchContent_Declare(
        JoltPhysics
        GIT_REPOSITORY "https://github.com/jrouwe/JoltPhysics"
        GIT_TAG "v5.3.0"
        SOURCE_SUBDIR "Build"
)
FetchContent_MakeAvailable(JoltPhysics)

# 1. Set VULKAN_SDK_PATH in .env.cmake to target specific vulkan version
if (DEFINED VULKAN_SDK_PATH)
    set(Vulkan_INCLUDE_DIRS "${VULKAN_SDK_PATH}/Include") # 1.1 Make sure this include path is correct
    set(Vulkan_LIBRARIES "${VULKAN_SDK_PATH}/Lib") # 1.2 Make sure lib path is correct
    set(Vulkan_FOUND "True")
else()
    find_package(Vulkan REQUIRED) # throws error if could not find Vulkan
    message(STATUS "Found Vulkan: $ENV{VULKAN_SDK}")
endif()
if (NOT Vulkan_FOUND)
    message(FATAL_ERROR "Could not find Vulkan library!")
else()
    message(STATUS "Using vulkan lib at: ${Vulkan_LIBRARIES}")
endif()

# Get GLFW 3.4 from GitHub
include(FetchContent)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw
        GIT_TAG 3.4
)

FetchContent_GetProperties(glfw)
if(NOT glfw_POPULATED)
    FetchContent_MakeAvailable(glfw)

    set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
    set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
    set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
    set(GLFW_BUILD_SHARED_LIBS ON) # <--- IMPORTANT
endif()

message(STATUS "EXTERNALS SOURCES: ${BUILTIN_EXTERNALS}")

add_library(Dependencies ${BUILTIN_EXTERNALS})

target_link_libraries(Dependencies PUBLIC glfw Jolt vulkan-1)

target_link_directories(Dependencies PUBLIC ${Vulkan_LIBRARIES})

target_include_directories(Dependencies PUBLIC 
        ${PROJECT_SOURCE_DIR}/External/im_nodeflow/include 
        ${PROJECT_SOURCE_DIR}/External/im_gui)
target_include_directories(Dependencies PUBLIC
        ${Vulkan_LIBRARIES}
        ${Vulkan_INCLUDE_DIRS}
        ${IM_GUI_PATH}
        ${IM_NODEFLOW_PATH})