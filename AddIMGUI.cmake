# Dear ImGui
set(IMGUI_DIR external/im_gui)
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
set(IMNODEFLOW_DIR external/im_nodeflow)

# SET SOURCE FILES FOR PROJECT
file(GLOB_RECURSE IMNODEFLOW_LIB ${IMNODEFLOW_DIR}/src/*.cpp 
        ${IMNODEFLOW_DIR}/src/*.h 
        ${IMNODEFLOW_DIR}/src/*.inl)

include_directories(${IMNODEFLOW_DIR}/include)

#STB Image
include_directories(external/)