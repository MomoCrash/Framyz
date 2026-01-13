#include "Window.h"

#include "Inputs.h"

Window::Window(const char* title, const int width, const int height)
    : m_title(title), m_width(width), m_height(height)
{
    
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    m_window = glfwCreateWindow(m_width, m_height, m_title, nullptr, nullptr);

    glfwSetWindowFocusCallback(m_window, SetFocusCallback);
    glfwSetKeyCallback(m_window, SetKeyCallback);
    glfwSetCharCallback(m_window, SetCharCallback);
    glfwSetMouseButtonCallback(m_window, SetMouseButtonCallback);
    glfwSetScrollCallback(m_window, SetScrollCallback);
    
}

Window::~Window()
{
    glfwDestroyWindow(m_window);

}

GLFWwindow* Window::GetWindow()
{
    return m_window;
}

void Window::SetInputMode(InputMode mode, CursorState state) {
    glfwSetInputMode(m_window, static_cast<int>(mode), static_cast<int>(state));
}

void Window::SetKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (action == GLFW_REPEAT)
        return;
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mode);
    Input::UpdateKey(static_cast<Input::KeyCode>(key), action == GLFW_PRESS ? Input::JUST_PRESSED : Input::JUST_RELEASED);
}

void Window::SetCharCallback(GLFWwindow *window, unsigned int c) {
    ImGui_ImplGlfw_CharCallback(window, c);
    
}

void Window::SetMouseButtonCallback(GLFWwindow *window, int button, int action, int mode) {
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mode);
    Input::UpdateMouse(static_cast<Input::KeyMouseCode>(button), action == GLFW_PRESS ? Input::JUST_PRESSED : Input::JUST_RELEASED);
}

void Window::SetScrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
    Input::UpdateScroll(xoffset, yoffset);
}

void Window::SetFocusCallback(GLFWwindow *window, int focused) {
    glfwWindowHint(GLFW_FOCUSED, focused);
    if (focused)
    {
        std::cout << "Focused" << std::endl;
        glfwSetKeyCallback(window, SetKeyCallback);
        glfwSetCharCallback(window, SetCharCallback);
        glfwSetMouseButtonCallback(window, SetMouseButtonCallback);
        glfwSetScrollCallback(window, SetScrollCallback);
    }
    ImGui_ImplGlfw_WindowFocusCallback(window, focused);
}
