list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(./.env.cmake OPTIONAL RESULT_VARIABLE LOCAL_ENV)
message(STATUS "Local .env.cmake: ${LOCAL_ENV}")

cmake_minimum_required(VERSION 3.11.0)

set(NAME FramyzEngine)
project(${NAME} VERSION 0.0.1)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

### Compilition defines
add_compile_definitions(FRAMYZ_EDITOR)

### Use the correct CMake Generator
message(STATUS "Using ${CMAKE_GENERATOR}")
if (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
    if (NOT MINGW_PATH)
        message(FATAL_ERROR "MINGW_PATH not set in .env.cmake")
    endif()
    set(USE_MINGW "True")
    set(CMAKE_C_COMPILER ${MINGW_PATH}/bin/gcc.exe)
    set(CMAKE_CXX_COMPILER  ${MINGW_PATH}/bin/g++.exe)
endif()
if (MSVC)
    message(STATUS "Forcing dynamic runtime (/MD and /MDd) for MSVC")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL" CACHE STRING "" FORCE)
endif()
if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL" CACHE STRING "" FORCE)
endif()
message(STATUS "MSVC Runtime: ${CMAKE_MSVC_RUNTIME_LIBRARY}")

### Add files of project
add_subdirectory(External)

add_subdirectory(Tools)

add_subdirectory(Render)

add_subdirectory(Engine)

add_subdirectory(Application)

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
endif ()

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
        DEPENDS ${SPIRV_BINARY_FILES})