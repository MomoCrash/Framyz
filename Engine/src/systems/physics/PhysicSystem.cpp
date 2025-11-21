// Created by Ethan G. on 22/06/2025.

#include "PhysicSystem.h"

#include "../../GameManager.h"
#include "../../ECS/components/physics/BoxCollider3D.h"
#include "../../ECS/components/physics/Rigidbody3D.h"
#include "../../ECS/components/physics/SphereCollider3D.h"

PhysicSystem::PhysicSystem() : m_physicsSystem(nullptr), m_broadPhaseLayer(nullptr), m_objVsBroadPhaseFilter(nullptr),
                               m_objVsObjFilter(nullptr), m_tempAllocator(nullptr), m_jobSystem(nullptr) {
}

PhysicSystem::~PhysicSystem() = default;

void PhysicSystem::update() {
	
}

void PhysicSystem::create() {
	BaseSystem::create();
	
    // Register allocation hook. 
	RegisterDefaultAllocator();

	// Install trace and assert callbacks
	Trace = TraceImpl;
	JPH_IF_ENABLE_ASSERTS(AssertFailed = AssertFailedImpl;)

	// Create a factory
	Factory::sInstance = new Factory();

	// If you have your own custom shape before calling this function.
	// If you implement your own default material (PhysicsMaterial::sDefault) before this function
	RegisterTypes();

	// We need a temp allocator for temporary allocations during the physics update. We're
	// pre-allocating 10 MB to avoid having to do allocations during the physics update.
	 m_jobSystem = new JobSystemThreadPool(cMaxPhysicsJobs, cMaxPhysicsBarriers, thread::hardware_concurrency() - 1);
	
	// We need a job system that will execute physics jobs on multiple threads. Typically
	// you would implement the JobSystem interface yourself and let Jolt Physics run on top
	// of your own job scheduler. JobSystemThreadPool is an example implementation.
	m_tempAllocator = new TempAllocatorImpl(10 * 1024 * 1024);
	
	// This is the max amount of rigid bodies that you can add to the physics system. If you try to add more you'll get an error.
	// For a real project use 65536.
#ifdef FRAMYZ_EDITOR
	const uint cMaxBodies = 1024;
#else
	const uint cMaxBodies = 65536;
#endif

	// This determines how many mutexes to allocate to protect rigid bodies from concurrent access. Set it to 0 for the default settings.
	const uint cNumBodyMutexes = 0;

	// This is the max amount of body pairs that can be queued at any time (the broad phase will detect overlapping
	// body pairs based on their bounding boxes and will insert them into a queue for the narrowphase). If you make this buffer
	// too small the queue will fill up and the broad phase jobs will start to do narrow phase work. This is slightly less efficient.
	// Note: This value is low because this is a simple test. For a real project use something in the order of 65536.
#ifdef FRAMYZ_EDITOR
    constexpr uint cMaxBodyPairs = 1024;
#else
	const uint cMaxBodyPairs = 65536;
#endif
	
	// This is the maximum size of the contact constraint buffer. If more contacts (collisions between bodies) are detected than this
	// number then these contacts will be ignored and bodies will start interpenetrating / fall through the world.
	// Note: This value is low because this is a simple test. For a real project use something in the order of 10240.
#ifdef FRAMYZ_EDITOR
    constexpr uint cMaxContactConstraints = 1024;
#else
	const uint cMaxContactConstraints = 10240;
#endif

	// Create mapping table from object layer to broadphase layer
	// Note: As this is an interface, PhysicsSystem will take a reference to this so this instance needs to stay alive!
	// Also have a look at BroadPhaseLayerInterfaceTable or BroadPhaseLayerInterfaceMask for a simpler interface.
	// HERE REPLACED BY m_broadPhaseLayer
	m_broadPhaseLayer = new BPLayerInterfaceImpl();
	
	// Create class that filters object vs broadphase layers
	// Note: As this is an interface, PhysicsSystem will take a reference to this so this instance needs to stay alive!
	// Also have a look at ObjectVsBroadPhaseLayerFilterTable or ObjectVsBroadPhaseLayerFilterMask for a simpler interface.
	// HERE REPLACED BY m_objVsBroadPhaseFilter
	m_objVsBroadPhaseFilter = new ObjectVsBroadPhaseLayerFilterImpl();
	// Create class that filters object vs object layers
	// Note: As this is an interface, PhysicsSystem will take a reference to this so this instance needs to stay alive!
	// Also have a look at ObjectLayerPairFilterTable or ObjectLayerPairFilterMask for a simpler interface.
	m_objVsObjFilter = new CollisionPairFilter();
	
	// Now we can create the actual physics system.
	m_physicsSystem = new PhysicsSystem();
	m_physicsSystem->Init(
		cMaxBodies, cNumBodyMutexes,
		cMaxBodyPairs, cMaxContactConstraints,
		
		*m_broadPhaseLayer,
		*m_objVsBroadPhaseFilter,
		*m_objVsObjFilter
		);

	// A body activation listener gets notified when bodies activate and go to sleep
	// Note that this is called from a job so whatever you do here needs to be thread safe.
	// Registering one is entirely optional.
	m_activationListener = new MyBodyActivationListener();
	m_physicsSystem->SetBodyActivationListener(m_activationListener);

	// A contact listener gets notified when bodies (are about to) collide, and when they separate again.
	// Note that this is called from a job so whatever you do here needs to be thread safe.
	// Registering one is entirely optional.
	m_contactListener = new MyContactListener();
	m_physicsSystem->SetContactListener(m_contactListener);

	// We simulate the physics world in discrete time steps. 60 Hz is a good rate to update the physics system.

	// Optional step: Before starting the physics simulation you can optimize the broad phase. This improves collision detection performance (it's pointless here because we only have 2 bodies).
	// You should definitely not call this every frame or when e.g. streaming in a new level section as it is an expensive operation.
	// Instead insert all new objects in batches instead of 1 at a time to keep the broad phase efficient.
	m_physicsSystem->OptimizeBroadPhase();
	
}

void PhysicSystem::fixedUpdate() {
	BodyInterface &body_interface = m_physicsSystem->GetBodyInterface();

	for (int i = 0; i < m_manager->getEntityCount(); i++) {
        
		Entity* entity = m_manager->getEntity(i);
		if (entity->hasComponent(Rigidbody3D::ComponentMask)) {
		
			Rigidbody3D* body = reinterpret_cast<Rigidbody3D *>(entity->getComponent(Rigidbody3D::ComponentMask));
			if (!body->IsValid) continue;
			
			RVec3 position = body_interface.GetCenterOfMassPosition(body->BodyID);
			body->GetOwner()->setLocalPosition(position.GetX(), position.GetY(), position.GetZ());
		}
		
	}
	// Step the world
	m_physicsSystem->Update(GameManager::GetClock().GetFixedDeltaTime(), m_collisionStep, m_tempAllocator, m_jobSystem);
	
}

void PhysicSystem::onComponentRegister(ComponentBase *component) {
	BaseSystem::onComponentRegister(component);

	if (component->is(BoxCollider3D::ComponentMask)) {
		BoxCollider3D *collider = dynamic_cast<BoxCollider3D*>(component);
		
	    BodyInterface &body_interface = m_physicsSystem->GetBodyInterface();
		
		BoxShapeSettings boxShape(Vec3(collider->Extend.x, collider->Extend.y, collider->Extend.z));
		boxShape.SetEmbedded();
		
		// Create the shape
		ShapeSettings::ShapeResult boxShapeResult = boxShape.Create();
		ShapeRefC floor_shape = boxShapeResult.Get(); // We don't expect an error here, but you can check floor_shape_result for HasError() / GetError()

		Transform& componentTransform = component->GetOwner()->getTransform();
		// Create the settings for the body itself. Note that here you can also set other properties like the restitution / friction.
		BodyCreationSettings boxSettings(floor_shape,
			RVec3(	componentTransform.getLocalPosition().x,
							componentTransform.getLocalPosition().y,
							componentTransform.getLocalPosition().z),
			Quat::sIdentity(), JPH::EMotionType::Static, Layers::NON_MOVING);

		// Create the actual rigid body
		collider->BodySettings = body_interface.CreateBody(boxSettings); // Note that if we run out of bodies this can return nullptr
		// Add it to the world
		body_interface.AddBody(collider->BodySettings->GetID(), collider->Activated);
		collider->BodyID = collider->BodySettings->GetID();

		if (component->GetOwner()->hasComponent(Rigidbody3D::ComponentMask)) {
			Rigidbody3D* body = dynamic_cast<Rigidbody3D *>(component->GetOwner()->getComponent(Rigidbody3D::ComponentMask));
			body->BodyID = collider->BodySettings->GetID();
			body->IsValid = true;
		}
	}

	if (component->is(SphereCollider3D::ComponentMask)) {
		SphereCollider3D *collider = dynamic_cast<SphereCollider3D*>(component);
		BodyInterface &body_interface = m_physicsSystem->GetBodyInterface();
		
		// Now create a dynamic body to bounce on the floor
		// Note that this uses the shorthand version of creating and adding a body to the world
		Transform& componentTransform = component->GetOwner()->getTransform();
		BodyCreationSettings sphereCollider(new SphereShape(0.5f),
				RVec3(componentTransform.getLocalPosition().x,
								componentTransform.getLocalPosition().y,
								componentTransform.getLocalPosition().z), Quat::sIdentity(),
						JPH::EMotionType::Dynamic, Layers::MOVING);
		// Create the actual rigid body
		collider->BodySettings = body_interface.CreateBody(sphereCollider); // Note that if we run out of bodies this can return nullptr
		// Add it to the world
		body_interface.AddBody(collider->BodySettings->GetID(), collider->Activated);
		collider->BodyID = collider->BodySettings->GetID();

		body_interface.SetLinearVelocity(collider->BodyID, Vec3(0.0f, 0.0f, 0.0f));
		
		if (component->GetOwner()->hasComponent(Rigidbody3D::ComponentMask)) {
			Rigidbody3D* body = dynamic_cast<Rigidbody3D *>(component->GetOwner()->getComponent(Rigidbody3D::ComponentMask));
			body->BodyID = collider->BodySettings->GetID();
			body->IsValid = true;
		}
	}
}

void PhysicSystem::onComponentUnregister(ComponentBase *component) {
	BaseSystem::onComponentUnregister(component);

	if (component->Mask & BoxCollider3D::ComponentMask) {
		
	}
}

void PhysicSystem::destroy() {
	BaseSystem::destroy();

	// // Remove the sphere from the physics system. Note that the sphere itself keeps all of its state and can be re-added at any time.
	// body_interface.RemoveBody(sphere_id);
	//
	// // Destroy the sphere. After this the sphere ID is no longer valid.
	// body_interface.DestroyBody(sphere_id);
	//
	// // Remove and destroy the floor
	// body_interface.RemoveBody(floor->GetID());
	// body_interface.DestroyBody(floor->GetID());

	// Unregisters all types with the factory and cleans up the default material
	UnregisterTypes();

	// Destroy the factory
	delete Factory::sInstance;
	Factory::sInstance = nullptr;
}

void PhysicSystem::preCreate() {
	BaseSystem::preCreate();
}
