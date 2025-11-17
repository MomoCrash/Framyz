//
// Created by Coline Constantinian on 09/11/2025.
//

#include "Inputs.h"

void Input::UpdateKey(KeyCode _key, KeyStatus _status) {
    m_statusKey[_key] =_status;
}

void Input::UpdateMouse(KeyMouseCode _key, KeyStatus _status) {
    m_statusMouse[_key] =_status;
}

void Input::UpdateScroll(float xoffset, float yoffset) {
    m_deltaMouseScrollX = xoffset;
    m_deltaMouseScrollY = yoffset;
    std::cout << m_deltaMouseScrollY << std::endl;
}

void Input::Update(GLFWwindow* _win) {
    for (auto kv: m_statusKey) {
        if (kv.second == JUST_RELEASED) {
            m_statusKey[kv.first] = RELEASED;
        }
    }
    for (auto kv: m_statusKey) {
        if (kv.second == JUST_PRESSED) {
            m_statusKey[kv.first] = PRESSED;
        }
    }
    for (auto kv: m_statusMouse) {
        if (kv.second == JUST_RELEASED) {
            m_statusMouse[kv.first] = RELEASED;
        }
    }
    for (auto kv: m_statusMouse) {
        if (kv.second == JUST_PRESSED) {
            m_statusMouse[kv.first] = PRESSED;
        }
    }
    double posx = m_mousePosX;
    double posy = m_mousePosY;
    glfwGetCursorPos(_win, &m_mousePosX, &m_mousePosY);
    if (m_init) {
        m_deltaMousePosX = m_mousePosX - posx;
        m_deltaMousePosY = m_mousePosY - posy;
    }
    m_deltaMouseScrollY = 0;
    m_deltaMouseScrollX = 0;
    m_init = true;
}

bool Input::IsKeyPressed(KeyCode _key) {
    if (m_statusKey.contains(_key)) {
        return m_statusKey[_key] > JUST_RELEASED ;
    }
    return false;
}

bool Input::IsKeyJustPressed(KeyCode _key) {
    if (m_statusKey.contains(_key)) {
        return m_statusKey[_key] == JUST_PRESSED ;
    }
    return false;
}

bool Input::IsKeyJustReleased(KeyCode _key) {
    if (m_statusKey.contains(_key)) {
        return m_statusKey[_key] == JUST_RELEASED ;
    }
    return false;
}

Input::KeyStatus Input::GetKeyStatus(KeyCode _key) {
    if (m_statusKey.contains(_key)) {
        return m_statusKey[_key];
    }
    return RELEASED;
}


bool Input::IsMouseButtonPressed(KeyMouseCode _key) {
    if (m_statusMouse.contains(_key)) {
        return m_statusMouse[_key] > JUST_RELEASED ;
    }
    return false;
}

bool Input::IsMouseButtonJustPressed(KeyMouseCode _key) {
    if (m_statusMouse.contains(_key)) {
        return m_statusMouse[_key] == JUST_PRESSED ;
    }
    return false;
}

bool Input::IsMouseButtonJustReleased(KeyMouseCode _key) {
    if (m_statusMouse.contains(_key)) {
        return m_statusMouse[_key] == JUST_RELEASED ;
    }
    return false;
}

Input::KeyStatus Input::GetMouseButtonStatus(KeyMouseCode _key) {
    if (m_statusMouse.contains(_key)) {
        return m_statusMouse[_key];
    }
    return RELEASED;
}

float Input::GetScrollOffsetX() {
    return m_deltaMouseScrollX;
}

float Input::GetScrollOffsetY() {
    return m_deltaMouseScrollY;
}

float Input::GetMouseX() {
    return m_mousePosX;
}

float Input::GetMouseY() {
    return m_mousePosY;
}

float Input::GetDeltaMouseX() {
    return m_deltaMousePosX;
}

float Input::GetDeltaMouseY() {
    return m_deltaMousePosY;
}
