#include "PhysicDebugLayer.h"

#include "../../EntityFactory.h"
#include "../../systems/RenderSystem.h"
#include "../../ECS/components/MeshRenderer.h"

PhysicDebugLayer::PhysicDebugLayer() : m_debugMesh(nullptr) {

    RenderSystem* renderer = GameManager::GetSystem<RenderSystem>();
    if (renderer != nullptr) {
        Entity *physicDebug = EntityFactory::CreateEntity();
        m_debugRenderer = EntityFactory::AttachComponent<MeshRenderer>(physicDebug);

        physicDebug->setDebugOnly(true);
        m_debugPipeline = renderer->WireframePipeline;
    }
    
}

PhysicDebugLayer::~PhysicDebugLayer() {
    if (m_debugMesh == nullptr) return;
    
    delete m_debugMesh;
    m_debugMesh = nullptr;
}

void PhysicDebugLayer::Update(RenderTarget *target) {

    if (m_debugData.Vertices.size() == 0) return;
    
    m_debugMesh = new Mesh("temp_debug_mesh", target->getRenderContext(), &m_debugData);
    m_debugRenderer->Object->setMesh(m_debugMesh);
    target->drawObject(*m_debugPipeline, *m_debugRenderer->Object);

    // TODO CLEAR NOT WORKING
    
}

void PhysicDebugLayer::Clear() {

    if (m_debugMesh == nullptr) return;
    
    delete m_debugMesh;
    m_debugMesh = nullptr;

    m_debugData.Vertices.clear();
    m_debugData.Indices.clear();
    
}
