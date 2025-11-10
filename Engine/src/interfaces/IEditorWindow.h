// Created by Ethan G. on 05/10/2025.

#ifndef IEDITORWINDOW_H
#define IEDITORWINDOW_H

#include "../framework.h"

class IEditorWindow {

public:
    virtual ~IEditorWindow() = default;

    virtual void open() = 0;
    virtual void close() = 0;
    virtual void draw() = 0;

};



#endif //IEDITORWINDOW_H
