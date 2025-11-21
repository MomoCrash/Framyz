// Created by Ethan G. on 05/10/2025.

#ifndef IEDITORWINDOW_H
#define IEDITORWINDOW_H

#include "../framework.h"

class IEditorWindow {

public:

    

    virtual void create() = 0;
    virtual void clear() = 0;
    
    virtual void open() = 0;
    virtual void close() = 0;
    virtual void draw() = 0;

    virtual ~IEditorWindow() = default;
    
};



#endif //IEDITORWINDOW_H
