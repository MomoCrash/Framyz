// Created by Ethan G. on 25/10/2025.

#ifndef BOXCOLLIDER3D_H
#define BOXCOLLIDER3D_H

#include "ComponentBase.h"

// An example contact listener
class MyContactListener : public ContactListener
{
public:
    // See: ContactListener
    virtual ValidateResult	OnContactValidate(const Body &inBody1, const Body &inBody2, RVec3Arg inBaseOffset, const CollideShapeResult &inCollisionResult) override
    {
        cout << "Contact validate callback" << endl;

        // Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
        return ValidateResult::AcceptAllContactsForThisBodyPair;
    }

    virtual void			OnContactAdded(const Body &inBody1, const Body &inBody2, const ContactManifold &inManifold, ContactSettings &ioSettings) override
    {
        cout << "A contact was added" << endl;
    }

    virtual void			OnContactPersisted(const Body &inBody1, const Body &inBody2, const ContactManifold &inManifold, ContactSettings &ioSettings) override
    {
        cout << "A contact was persisted" << endl;
    }

    virtual void			OnContactRemoved(const SubShapeIDPair &inSubShapePair) override
    {
        cout << "A contact was removed" << endl;
    }
};

// An example activation listener
class MyBodyActivationListener : public BodyActivationListener
{
public:
    virtual void		OnBodyActivated(const BodyID &inBodyID, uint64 inBodyUserData) override
    {
        cout << "A body got activated" << endl;
    }

    virtual void		OnBodyDeactivated(const BodyID &inBodyID, uint64 inBodyUserData) override
    {
        cout << "A body went to sleep" << endl;
    }
};

DECLARE_COMPONENT( BoxCollider3D, Component )
{
    BoxCollider3D();
    
    void EDITOR_Display() override;
    void instantiate() override;

    float FOV = 0;
    
};


#endif //BOXCOLLIDER3D_H
