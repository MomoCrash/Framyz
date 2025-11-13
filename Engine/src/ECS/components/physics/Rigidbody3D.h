// Created by Ethan G. on 02/11/2025.

#ifndef RIGIDBODY3D_H
#define RIGIDBODY3D_H

#include "../ComponentBase.h"

DECLARE_COMPONENT( Rigidbody3D, Component ) {
	
    Rigidbody3D();

	void EDITOR_Display()   override;
	void instantiate()      override;

    bool 					IsValid;
    JPH::BodyID 			BodyID;
	
};



#endif //RIGIDBODY3D_H
