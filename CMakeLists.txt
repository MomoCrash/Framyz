list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(./.env.cmake OPTIONAL RESULT_VARIABLE LOCAL_ENV)
message(STATUS "Local .env.cmake: ${LOCAL_ENV}")

cmake_minimum_required(VERSION 3.11.0)

set(NAME FramyzEngine)

# Application defines

add_compile_definitions(FRAMYZ_EDITOR)

message(STATUS "using ${CMAKE_GENERATOR}")
if (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
    if (NOT MINGW_PATH)
        message(FATAL_ERROR "MINGW_PATH not set in .env.cmake")
    endif()
    set(USE_MINGW "True")
    set(CMAKE_C_COMPILER ${MINGW_PATH}/bin/gcc.exe)
    set(CMAKE_CXX_COMPILER  ${MINGW_PATH}/bin/g++.exe)
endif()

project(${NAME} VERSION 0.0.1)

# Appliquer le runtime dynamique (/MD et /MDd) pour MSVC
if (MSVC)
    message(STATUS "Forcing dynamic runtime (/MD and /MDd) for MSVC")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL" CACHE STRING "" FORCE)
endif()

if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL" CACHE STRING "" FORCE)
endif()

message(STATUS "MSVC Runtime: ${CMAKE_MSVC_RUNTIME_LIBRARY}")

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

include(AddIMGUI.cmake)

include(AddJolt.cmake)

include_directories(external)

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
message(STATUS "SOURCES: ${SOURCES}")

add_executable(${PROJECT_NAME} ${SOURCES} ${IM_GUI_LIB} ${IMNODEFLOW_LIB})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/build")

if (WIN32)
    message(STATUS "CREATING BUILD FOR WINDOWS")

    if (USE_MINGW)
        target_include_directories(${PROJECT_NAME} PUBLIC
                ${MINGW_PATH}/include
        )
        target_link_directories(${PROJECT_NAME} PUBLIC
                ${MINGW_PATH}/lib
        )
    endif()

    target_include_directories(${PROJECT_NAME} PUBLIC
            ${PROJECT_SOURCE_DIR}/src
            ${Vulkan_INCLUDE_DIRS}
            ${IM_GUI_PATH}
            ${IM_NODEFLOW_PATH}
            ${JoltPhysics_SOURCE_DIR}/..
    )

    target_link_directories(${PROJECT_NAME} PUBLIC
            ${Vulkan_LIBRARIES}
    )

    target_link_libraries(${PROJECT_NAME} glfw Jolt vulkan-1)
elseif (UNIX)
    message(STATUS "CREATING BUILD FOR UNIX")
    target_include_directories(${PROJECT_NAME} PUBLIC
            ${PROJECT_SOURCE_DIR}/src
            ${JoltPhysics_SOURCE_DIR}/..
    )
    target_link_libraries(${PROJECT_NAME} glfw Jolt ${Vulkan_LIBRARIES})
endif()


############## Build SHADERS #######################

# Find all vertex and fragment sources within shaders directory
# taken from VBlancos vulkan tutorial
# https://github.com/vblanco20-1/vulkan-guide/blob/all-chapters/CMakeLists.txt
find_program(GLSL_VALIDATOR glslc ${VULKAN_SDK_PATH}/Bin/glslc.exe)

message(STATUS ${GLSL_VALIDATOR})

# get all .vert and .frag files in shaders directory
file(GLOB_RECURSE GLSL_SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/shaders/*.frag
        ${PROJECT_SOURCE_DIR}/shaders/*.vert
)

foreach(GLSL ${GLSL_SOURCE_FILES})
    get_filename_component(FILE_NAME ${GLSL} NAME)
    set(SPIRV ${PROJECT_SOURCE_DIR}/shaders/${FILE_NAME}.spv)
    add_custom_command(
            OUTPUT ${SPIRV}
            COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
            DEPENDS ${GLSL})
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
    message(STATUS "Created file : " ${SPIRV})
endforeach(GLSL)

add_custom_target(
        Shaders
        DEPENDS ${SPIRV_BINARY_FILES}
)