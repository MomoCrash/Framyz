// Created by Ethan G. on 22/06/2025.

#ifndef SCENEWINDOW_H
#define SCENEWINDOW_H

#include "../../framework.h"

class SceneWindow {

public:
    SceneWindow();
    ~SceneWindow();

    void draw(RenderWindow* window);

    VkDescriptorSet renderedImages[RenderWindow::MAX_FRAMES_IN_FLIGHT];

};



#endif //SCENEWINDOW_H
