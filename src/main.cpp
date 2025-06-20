// Created by Ethan G. on 20/06/2025.

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Engine/engine.hpp"

int main() {

    // Call it to sync this window with the application
    Application::getInstance()->Initialize("Application");
    
    GuiHandler ui;

    Editor editor(&ui);

    while(!editor.isEditorClosed())
    {
        
        glfwPollEvents();
        
        editor.draw();
        
    }

    vkDeviceWaitIdle(Application::getInstance()->getDevice());
    
    return 0;
    
}
