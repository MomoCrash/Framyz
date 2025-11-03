cmake_minimum_required(VERSION 3.31)

## Application
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/Application/src/*.cpp ${PROJECT_SOURCE_DIR}/Application/src/*.inl ${PROJECT_SOURCE_DIR}/Application/src/*.c)
file(GLOB_RECURSE HEADERS ${PROJECT_SOURCE_DIR}/Application/src/*.h)
message(STATUS "APPLICATION SOURCES: ${SOURCES}")
message(STATUS "APPLICATION HEADERS: ${HEADERS}")

add_executable(Application ${SOURCES} ${HEADERS})

## Link files
target_compile_features(Application PUBLIC cxx_std_20)
set_property(TARGET Application PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/build")
target_include_directories(Application PUBLIC ${PROJECT_SOURCE_DIR}/Application/src)

## Link libraries
target_include_directories(Application PUBLIC ${PROJECT_SOURCE_DIR}/Engine/src)
target_link_libraries(Application PUBLIC Engine)