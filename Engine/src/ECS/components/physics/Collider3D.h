#pragma once

#include "../ComponentBase.h"

struct Collider3D {

    Collider3D();

    JPH::EActivation    Activated;
    JPH::EMotionType    MotionType;
    JPH::ObjectLayer    Layer;
    
    JPH::Body*          BodySettings;
    JPH::BodyID         BodyID;
};
