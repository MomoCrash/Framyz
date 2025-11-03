## Tools files
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/Render/src/*.cpp ${PROJECT_SOURCE_DIR}/Tools/src/*.inl ${PROJECT_SOURCE_DIR}/Tools/src/*.c)
file(GLOB_RECURSE HEADERS ${PROJECT_SOURCE_DIR}/Render/src/*.h)
message(STATUS "RENDER SOURCES: ${SOURCES}")
message(STATUS "RENDER HEADERS: ${HEADERS}")

add_library(Render ${SOURCES} ${HEADERS})

target_include_directories(Render PUBLIC ${PROJECT_SOURCE_DIR}/Tools/src)
target_link_libraries(Render PUBLIC Tools)
