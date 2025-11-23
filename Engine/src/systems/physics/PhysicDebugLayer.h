#pragma once


#pragma once

#ifndef JPH_DEBUG_RENDERER
	#error This file should only be included when JPH_DEBUG_RENDERER is defined
#endif // !JPH_DEBUG_RENDERER

#include "../../framework.h"

#include <Jolt/Jolt.h>
#include <Jolt/Renderer/DebugRenderer.h>
#include "Jolt/Renderer/DebugRendererSimple.h"
#include <Jolt/Math/Math.h>
#include "Jolt/Math/Real.h"

class MeshRenderer;

class PhysicDebugLayer : public JPH::DebugRendererSimple
{
public:

	PhysicDebugLayer();
	~PhysicDebugLayer() override;
	
	virtual void DrawLine(JPH::RVec3Arg inFrom, JPH::RVec3Arg inTo, JPH::ColorArg inColor) override
	{
		int vert1 = m_debugData.Vertices.size();
		m_debugData.Vertices.emplace_back(inFrom.GetX(), inFrom.GetY(), inFrom.GetZ(), 0, 0, 0, 0, 0);
		int vert2 = m_debugData.Vertices.size();
		m_debugData.Vertices.emplace_back(inTo.GetX(), inTo.GetY(), inTo.GetZ(), 0, 0, 0, 0, 0);

		m_debugData.Indices.emplace_back(vert1);
		m_debugData.Indices.emplace_back(vert2);
		m_debugData.Indices.emplace_back(vert1);

	}

	virtual void DrawTriangle(JPH::RVec3Arg inV1, JPH::RVec3Arg inV2, JPH::RVec3Arg inV3, JPH::ColorArg inColor, ECastShadow inCastShadow) override
	{
		// Implement
	}

	virtual void DrawText3D(JPH::RVec3Arg inPosition, const std::string_view &inString, JPH::ColorArg inColor, float inHeight) override
	{
		// Implement
	}

	void Update(RenderTarget* target);
	void Clear();

	

private:
	MeshData		m_debugData;
	Mesh*			m_debugMesh;
	MeshRenderer*	m_debugRenderer;

	RenderPipeline*	m_debugPipeline;
};