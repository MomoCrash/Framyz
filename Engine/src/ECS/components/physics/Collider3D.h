#pragma once

#include "../../../framework.h"
#include "../../../systems/PhysicSystem.h"
#include "../ComponentBase.h"

struct Collider3D {

    Collider3D();

    JPH::EActivation    Activated   = JPH::EActivation::Activate;
    JPH::EMotionType    MotionType  = JPH::EMotionType::Dynamic;
    JPH::ObjectLayer    Layer       = Layers::MOVING;
    
    JPH::Body*          BodySettings;
    JPH::BodyID         BodyID;
};
