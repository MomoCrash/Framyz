cmake_minimum_required(VERSION 3.31)

## Render files
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/Tools/src/*.cpp ${PROJECT_SOURCE_DIR}/Tools/src/*.inl ${PROJECT_SOURCE_DIR}/Tools/src/*.c)
file(GLOB_RECURSE HEADERS ${PROJECT_SOURCE_DIR}/Tools/src/*.h)
message(STATUS "TOOLS SOURCES: ${SOURCES}")
message(STATUS "TOOLS HEADERS: ${HEADERS}")

add_library(Tools ${SOURCES} ${HEADERS})

target_include_directories(Tools PUBLIC ${PROJECT_SOURCE_DIR}/External/)
target_link_libraries(Tools PUBLIC Dependencies)
