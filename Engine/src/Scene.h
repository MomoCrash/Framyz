// Created by Ethan G. on 22/06/2025.

#ifndef SCENE_H
#define SCENE_H

class Scene {
    
public:
    virtual ~Scene() = default;
    void virtual OnEnter() = 0;
    void virtual OnFixedUpdate() = 0;
    void virtual OnUpdate() = 0;
    void virtual OnExit() = 0;
    void virtual Render() = 0;
    
};

#endif //SCENE_H
