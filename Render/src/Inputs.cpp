//
// Created by Coline Constantinian on 09/11/2025.
//

#include "Inputs.h"
std::map<Input::KeyCode, Input::KeyStatus>         Input::m_statusKey;
std::map<Input::KeyMouseCode, Input::KeyStatus>    Input::m_statusMouse;
bool    Input::m_init            = false;
double  Input::m_mousePosX       = 0;
double  Input::m_mousePosY       = 0;
double  Input::m_deltaMousePosX  = 0;
double  Input::m_deltaMousePosY  = 0;

void Input::UpdateKey(KeyCode _key, KeyStatus _status) {
    m_statusKey[_key] =_status;
}

void Input::UpdateMouse(KeyMouseCode _key, KeyStatus _status) {
    m_statusMouse[_key] =_status;
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

glm::dvec2 Input::GetMousePosition() {
    return {m_mousePosX, m_mousePosY};
}

glm::dvec2 Input::GetDeltaMousePosition() {
    return {m_deltaMousePosX, m_deltaMousePosY};
}

