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

class PhysicDebugLayer : public JPH::DebugRendererSimple
{
public:

	PhysicDebugLayer() {
	}
	
	virtual void DrawLine(JPH::RVec3Arg inFrom, JPH::RVec3Arg inTo, JPH::ColorArg inColor) override
	{
		// Implement
	}

	virtual void DrawTriangle(JPH::RVec3Arg inV1, JPH::RVec3Arg inV2, JPH::RVec3Arg inV3, JPH::ColorArg inColor, ECastShadow inCastShadow) override
	{
		// Implement
	}

	virtual void DrawText3D(JPH::RVec3Arg inPosition, const std::string_view &inString, JPH::ColorArg inColor, float inHeight) override
	{
		// Implement
	}
};