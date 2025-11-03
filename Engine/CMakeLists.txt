## Tools files
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/Engine/src/*.cpp ${PROJECT_SOURCE_DIR}/Engine/src/*.inl ${PROJECT_SOURCE_DIR}/Engine/src/*.c)
file(GLOB_RECURSE HEADERS ${PROJECT_SOURCE_DIR}/Engine/src/*.h)
message(STATUS "RENDER SOURCES: ${SOURCES}")
message(STATUS "RENDER HEADERS: ${HEADERS}")

add_library(Engine ${SOURCES} ${HEADERS})

target_include_directories(Engine PUBLIC ${PROJECT_SOURCE_DIR}/Render/src)
target_link_libraries(Engine PUBLIC Render)
